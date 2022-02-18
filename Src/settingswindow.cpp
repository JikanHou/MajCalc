#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::SettingsWindow){
    ui -> setupUi(this);
    connect(ui -> confirm, &QAction:: triggered, this, &SettingsWindow:: settingsWindow_ConfirmButtonClicked);
    connect(ui -> cancel, &QAction:: triggered, this, &SettingsWindow:: settingsWindow_CancelButtonClicked);
    for(int i = 0; i < 4; i ++)
        uma[i] = findChild<QLineEdit*>(QString("uma") + char('1' + i));
    restoreSettings();
}

SettingsWindow::~SettingsWindow(){
    delete ui;
}

QStringList SettingsWindow:: getUma(){
    QStringList res;
    bool ok;
    for(int i = 0; i < 4; i ++){
        uma[i]->text().toInt(&ok);
        if (ok && !uma[i] -> text().isEmpty())
            res.push_back(uma[i] -> text());
        else
            res.push_back(uma[i] -> placeholderText());
    }
    return res;
}

int SettingsWindow:: getRate(){
    bool suc;
    int res = ui -> rate -> text().toInt(&suc);
    return !suc? ui -> rate -> placeholderText().toInt(): res;
}

QStringList SettingsWindow:: getPlayerList(){
    return ui -> playerText -> toPlainText().replace(" ", "_").split("\n");
}

int SettingsWindow:: getMochi(){
    bool suc;
    int res = ui -> mochi -> text().toInt(&suc);
    return !suc? ui -> mochi -> placeholderText().toInt(): res;;
}

int SettingsWindow:: getKaeshi(){
    bool suc;
    int res = ui -> kaeshi -> text().toInt(&suc);
    return !suc? ui -> kaeshi -> placeholderText().toInt(): res;
}

void SettingsWindow:: restoreSettings(){
    QSettings settings("HouJikan", "MJCalc");
    ui -> mochi -> setText(settings.value("mochi", "25000").toString());
    ui -> kaeshi -> setText(settings.value("kaeshi", "25000").toString());
    ui -> rate -> setText(settings.value("rate", "1000").toString());
    QStringList umas = settings.value("uma").toStringList();
    if (umas.isEmpty())
        umas << "20" << "10" << "-10" << "-20";
    for(int i = 0; i < 4; i ++)
        uma[i] -> setText(umas[i]);
    QString temp;
    QStringList playerList = settings.value("playerList").toStringList();
    for (QString it: playerList)
        if (!it.isEmpty() && it != "\n")
            temp += it + "\n";
    ui -> playerText -> setPlainText(temp);
}

void SettingsWindow:: mainWindow_SettingsWindowClicked(){
    setWindowModality(Qt:: ApplicationModal);
    show();
}

void SettingsWindow:: closeEvent(QCloseEvent *e){
    e -> ignore();
    settingsWindow_CancelButtonClicked();
}

void SettingsWindow:: settingsWindow_CancelButtonClicked(){
    hide();
    restoreSettings();
    setWindowModality(Qt:: NonModal);
}

void SettingsWindow:: settingsWindow_ConfirmButtonClicked(){
    QSettings settings("HouJikan", "MJCalc");
    settings.setValue("mochi", getMochi());
    settings.setValue("kaeshi", getKaeshi());
    settings.setValue("uma", getUma());
    settings.setValue("rate", getRate());
    settings.setValue("playerList", getPlayerList());
    hide();
    setWindowModality(Qt:: NonModal);
    emit settingsWindow_ConfirmButtonClickedSignal(getPlayerList());
}
