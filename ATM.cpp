// ============================================================
//  ATM Application — wxWidgets 3.2
//  Build (Linux):
//    g++ atm_wxwidgets.cpp `wx-config --cxxflags --libs` -o atm
//  Build (Windows, MinGW):
//    g++ atm_wxwidgets.cpp $(wx-config --cxxflags --libs) -o atm.exe
// ============================================================

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/font.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <wx/notebook.h>

// ─── Data model (same logic as original console ATM) ────────
struct Transaction {
    wxString type;   // "Deposit" or "Withdrawal"
    double   amount;
    double   balanceAfter;
};

class ATM {
public:
    ATM() : balance(4500.0) {}

    double getBalance() const { return balance; }

    // Returns true on success, false + reason on failure
    bool deposit(double money, wxString &msg) {
        if (money <= 0) { msg = "Amount must be greater than zero."; return false; }
        balance += money;
        Transaction t{"Deposit", money, balance};
        history.push_back(t);
        msg = wxString::Format("PKR %.2f deposited successfully.", money);
        return true;
    }

    bool withdraw(double money, wxString &msg) {
        if (money <= 0)       { msg = "Invalid withdrawal amount.";              return false; }
        if (money > balance)  { msg = wxString::Format("Insufficient funds. Available: PKR %.2f", balance); return false; }
        balance -= money;
        Transaction t{"Withdrawal", money, balance};
        history.push_back(t);
        msg = wxString::Format("PKR %.2f withdrawn successfully.", money);
        return true;
    }

    const std::vector<Transaction>& getHistory() const { return history; }

private:
    double balance;
    std::vector<Transaction> history;
};

// ─── IDs ────────────────────────────────────────────────────
enum {
    ID_BTN_BALANCE  = wxID_HIGHEST + 1,
    ID_BTN_DEPOSIT,
    ID_BTN_WITHDRAW,
    ID_BTN_HISTORY,
    ID_DO_DEPOSIT,
    ID_DO_WITHDRAW,
    NOTEBOOK_PAGE_BALANCE  = 0,
    NOTEBOOK_PAGE_DEPOSIT  = 1,
    NOTEBOOK_PAGE_WITHDRAW = 2,
    NOTEBOOK_PAGE_HISTORY  = 3,
};

// ─── Main frame ─────────────────────────────────────────────
class ATMFrame : public wxFrame {
public:
    ATMFrame();

private:
    ATM atm;

    // UI widgets we need to reference later
    wxNotebook   *notebook;
    wxStaticText *balanceLabel;
    wxStaticText *balanceDepLabel;
    wxStaticText *balanceWdwLabel;
    wxTextCtrl   *depositInput;
    wxTextCtrl   *withdrawInput;
    wxListCtrl   *historyList;
    wxStatusBar  *statusBar;

    // Helpers
    wxPanel *MakeBalancePage(wxNotebook *nb);
    wxPanel *MakeDepositPage(wxNotebook *nb);
    wxPanel *MakeWithdrawPage(wxNotebook *nb);
    wxPanel *MakeHistoryPage(wxNotebook *nb);

    wxString FormatBalance(double v) {
        return wxString::Format("PKR %.2f", v);
    }

    void RefreshBalances() {
        wxString s = FormatBalance(atm.getBalance());
        balanceLabel->SetLabel(s);
        balanceDepLabel->SetLabel(s);
        balanceWdwLabel->SetLabel(s);
    }

    void RefreshHistory();

    // Event handlers
    void OnDeposit(wxCommandEvent &);
    void OnWithdraw(wxCommandEvent &);

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(ATMFrame, wxFrame)
    EVT_BUTTON(ID_DO_DEPOSIT,  ATMFrame::OnDeposit)
    EVT_BUTTON(ID_DO_WITHDRAW, ATMFrame::OnWithdraw)
wxEND_EVENT_TABLE()

// ─── Frame constructor ───────────────────────────────────────
ATMFrame::ATMFrame()
    : wxFrame(nullptr, wxID_ANY, "ATM Application",
              wxDefaultPosition, wxSize(620, 480))
{
    SetBackgroundColour(wxColour(22, 33, 62));   // dark navy

    // ── Menu bar ────────────────────────────────────────────
    wxMenuBar *mb = new wxMenuBar;
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "&Exit\tAlt+F4");
    mb->Append(fileMenu, "&File");
    SetMenuBar(mb);
    Bind(wxEVT_MENU, [](wxCommandEvent&){ wxExit(); }, wxID_EXIT);

    // ── Status bar ──────────────────────────────────────────
    statusBar = CreateStatusBar(2);
    statusBar->SetStatusWidths(2, new int[2]{-1, 160});
    statusBar->SetStatusText("Ready");
    statusBar->SetStatusText("SecureBank ATM v1.0", 1);

    // ── Notebook (tab control) ───────────────────────────────
    notebook = new wxNotebook(this, wxID_ANY);
    notebook->SetBackgroundColour(wxColour(22, 33, 62));

    notebook->AddPage(MakeBalancePage(notebook),  " Check Balance ");
    notebook->AddPage(MakeDepositPage(notebook),  " Deposit        ");
    notebook->AddPage(MakeWithdrawPage(notebook), " Withdraw       ");
    notebook->AddPage(MakeHistoryPage(notebook),  " History        ");

    wxBoxSizer *root = new wxBoxSizer(wxVERTICAL);
    root->Add(notebook, 1, wxEXPAND | wxALL, 8);
    SetSizer(root);

    Centre();
}

// ─── Balance page ────────────────────────────────────────────
wxPanel *ATMFrame::MakeBalancePage(wxNotebook *nb) {
    wxPanel *p = new wxPanel(nb);
    p->SetBackgroundColour(wxColour(15, 30, 60));

    wxBoxSizer *vs = new wxBoxSizer(wxVERTICAL);
    vs->AddSpacer(30);

    // Card box
    wxStaticBox  *box  = new wxStaticBox(p, wxID_ANY, " Available Balance ");
    box->SetForegroundColour(wxColour(160, 196, 232));
    wxStaticBoxSizer *card = new wxStaticBoxSizer(box, wxVERTICAL);
    card->AddSpacer(16);

    wxStaticText *lbl = new wxStaticText(p, wxID_ANY, "Account No: **** **** 4582",
                                         wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    lbl->SetForegroundColour(wxColour(120, 140, 180));
    card->Add(lbl, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
    card->AddSpacer(12);

    balanceLabel = new wxStaticText(p, wxID_ANY, FormatBalance(atm.getBalance()),
                                    wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    wxFont bf = balanceLabel->GetFont();
    bf.SetPointSize(28);
    bf.SetWeight(wxFONTWEIGHT_BOLD);
    balanceLabel->SetFont(bf);
    balanceLabel->SetForegroundColour(wxColour(78, 205, 196));
    card->Add(balanceLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 20);

    vs->Add(card, 0, wxEXPAND | wxLEFT | wxRIGHT, 40);
    p->SetSizer(vs);
    return p;
}

// ─── Deposit page ────────────────────────────────────────────
wxPanel *ATMFrame::MakeDepositPage(wxNotebook *nb) {
    wxPanel *p = new wxPanel(nb);
    p->SetBackgroundColour(wxColour(15, 30, 60));

    wxBoxSizer *vs = new wxBoxSizer(wxVERTICAL);
    vs->AddSpacer(24);

    wxStaticText *title = new wxStaticText(p, wxID_ANY, "Deposit Money");
    wxFont tf = title->GetFont(); tf.SetPointSize(14); tf.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(tf);
    title->SetForegroundColour(wxColour(200, 220, 255));
    vs->Add(title, 0, wxLEFT, 30);
    vs->AddSpacer(16);

    wxStaticText *lbl = new wxStaticText(p, wxID_ANY, "Amount to Deposit (PKR):");
    lbl->SetForegroundColour(wxColour(140, 160, 200));
    vs->Add(lbl, 0, wxLEFT, 30);
    vs->AddSpacer(6);

    depositInput = new wxTextCtrl(p, wxID_ANY, "", wxDefaultPosition, wxSize(260, -1));
    vs->Add(depositInput, 0, wxLEFT, 30);
    vs->AddSpacer(14);

    wxButton *btn = new wxButton(p, ID_DO_DEPOSIT, "  Deposit Funds");
    btn->SetBackgroundColour(wxColour(233, 69, 96));
    btn->SetForegroundColour(*wxWHITE);
    vs->Add(btn, 0, wxLEFT, 30);
    vs->AddSpacer(20);

    wxStaticLine *line = new wxStaticLine(p);
    vs->Add(line, 0, wxEXPAND | wxLEFT | wxRIGHT, 30);
    vs->AddSpacer(14);

    wxStaticText *blbl = new wxStaticText(p, wxID_ANY, "Current Balance:");
    blbl->SetForegroundColour(wxColour(140, 160, 200));
    vs->Add(blbl, 0, wxLEFT, 30);

    balanceDepLabel = new wxStaticText(p, wxID_ANY, FormatBalance(atm.getBalance()));
    wxFont bf = balanceDepLabel->GetFont(); bf.SetPointSize(18); bf.SetWeight(wxFONTWEIGHT_BOLD);
    balanceDepLabel->SetFont(bf);
    balanceDepLabel->SetForegroundColour(wxColour(46, 213, 115));
    vs->Add(balanceDepLabel, 0, wxLEFT, 30);

    p->SetSizer(vs);
    return p;
}

// ─── Withdraw page ───────────────────────────────────────────
wxPanel *ATMFrame::MakeWithdrawPage(wxNotebook *nb) {
    wxPanel *p = new wxPanel(nb);
    p->SetBackgroundColour(wxColour(15, 30, 60));

    wxBoxSizer *vs = new wxBoxSizer(wxVERTICAL);
    vs->AddSpacer(24);

    wxStaticText *title = new wxStaticText(p, wxID_ANY, "Withdraw Money");
    wxFont tf = title->GetFont(); tf.SetPointSize(14); tf.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(tf);
    title->SetForegroundColour(wxColour(200, 220, 255));
    vs->Add(title, 0, wxLEFT, 30);
    vs->AddSpacer(16);

    wxStaticText *lbl = new wxStaticText(p, wxID_ANY, "Amount to Withdraw (PKR):");
    lbl->SetForegroundColour(wxColour(140, 160, 200));
    vs->Add(lbl, 0, wxLEFT, 30);
    vs->AddSpacer(6);

    withdrawInput = new wxTextCtrl(p, wxID_ANY, "", wxDefaultPosition, wxSize(260, -1));
    vs->Add(withdrawInput, 0, wxLEFT, 30);
    vs->AddSpacer(14);

    wxButton *btn = new wxButton(p, ID_DO_WITHDRAW, "  Withdraw Funds");
    btn->SetBackgroundColour(wxColour(233, 69, 96));
    btn->SetForegroundColour(*wxWHITE);
    vs->Add(btn, 0, wxLEFT, 30);
    vs->AddSpacer(20);

    wxStaticLine *line = new wxStaticLine(p);
    vs->Add(line, 0, wxEXPAND | wxLEFT | wxRIGHT, 30);
    vs->AddSpacer(14);

    wxStaticText *blbl = new wxStaticText(p, wxID_ANY, "Current Balance:");
    blbl->SetForegroundColour(wxColour(140, 160, 200));
    vs->Add(blbl, 0, wxLEFT, 30);

    balanceWdwLabel = new wxStaticText(p, wxID_ANY, FormatBalance(atm.getBalance()));
    wxFont bf = balanceWdwLabel->GetFont(); bf.SetPointSize(18); bf.SetWeight(wxFONTWEIGHT_BOLD);
    balanceWdwLabel->SetFont(bf);
    balanceWdwLabel->SetForegroundColour(wxColour(233, 69, 96));
    vs->Add(balanceWdwLabel, 0, wxLEFT, 30);

    p->SetSizer(vs);
    return p;
}

// ─── History page ────────────────────────────────────────────
wxPanel *ATMFrame::MakeHistoryPage(wxNotebook *nb) {
    wxPanel *p = new wxPanel(nb);
    p->SetBackgroundColour(wxColour(15, 30, 60));

    wxBoxSizer *vs = new wxBoxSizer(wxVERTICAL);
    vs->AddSpacer(16);

    wxStaticText *title = new wxStaticText(p, wxID_ANY, "Transaction History");
    wxFont tf = title->GetFont(); tf.SetPointSize(14); tf.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(tf);
    title->SetForegroundColour(wxColour(200, 220, 255));
    vs->Add(title, 0, wxLEFT, 16);
    vs->AddSpacer(10);

    historyList = new wxListCtrl(p, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                  wxLC_REPORT | wxLC_SINGLE_SEL | wxBORDER_NONE);
    historyList->SetBackgroundColour(wxColour(13, 27, 42));
    historyList->SetForegroundColour(wxColour(160, 196, 232));

    historyList->AppendColumn("Type",            wxLIST_FORMAT_LEFT,  120);
    historyList->AppendColumn("Amount (PKR)",    wxLIST_FORMAT_RIGHT, 130);
    historyList->AppendColumn("Balance After",   wxLIST_FORMAT_RIGHT, 140);

    vs->Add(historyList, 1, wxEXPAND | wxALL, 16);
    p->SetSizer(vs);
    return p;
}

void ATMFrame::RefreshHistory() {
    historyList->DeleteAllItems();
    const auto &h = atm.getHistory();
    for (int i = (int)h.size() - 1; i >= 0; --i) {
        long idx = historyList->InsertItem(0, h[i].type);
        historyList->SetItem(idx, 1, wxString::Format("%.2f", h[i].amount));
        historyList->SetItem(idx, 2, wxString::Format("%.2f", h[i].balanceAfter));
    }
}

// ─── Event handlers ──────────────────────────────────────────
void ATMFrame::OnDeposit(wxCommandEvent &) {
    wxString raw = depositInput->GetValue().Trim();
    double amount = 0.0;
    if (!raw.ToDouble(&amount)) {
        wxMessageBox("Please enter a valid numeric amount.", "Input Error",
                     wxICON_ERROR | wxOK, this);
        return;
    }
    wxString msg;
    if (atm.deposit(amount, msg)) {
        depositInput->Clear();
        RefreshBalances();
        RefreshHistory();
        wxMessageBox(msg, "Success", wxICON_INFORMATION | wxOK, this);
        statusBar->SetStatusText("Deposit of PKR " + wxString::Format("%.2f", amount) + " completed.");
    } else {
        wxMessageBox(msg, "Error", wxICON_ERROR | wxOK, this);
    }
}

void ATMFrame::OnWithdraw(wxCommandEvent &) {
    wxString raw = withdrawInput->GetValue().Trim();
    double amount = 0.0;
    if (!raw.ToDouble(&amount)) {
        wxMessageBox("Please enter a valid numeric amount.", "Input Error",
                     wxICON_ERROR | wxOK, this);
        return;
    }
    wxString msg;
    if (atm.withdraw(amount, msg)) {
        withdrawInput->Clear();
        RefreshBalances();
        RefreshHistory();
        wxMessageBox(msg, "Success", wxICON_INFORMATION | wxOK, this);
        statusBar->SetStatusText("Withdrawal of PKR " + wxString::Format("%.2f", amount) + " completed.");
    } else {
        wxMessageBox(msg, "Error", wxICON_ERROR | wxOK, this);
    }
}

// ─── Application entry point ─────────────────────────────────
class ATMApp : public wxApp {
public:
    bool OnInit() override {
        ATMFrame *frame = new ATMFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(ATMApp);