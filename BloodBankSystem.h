#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BloodBankSystem.h"

class BloodBankSystem : public QMainWindow
{
    Q_OBJECT

public:
    BloodBankSystem(QWidget *parent = nullptr);
    ~BloodBankSystem();

private:
    Ui::BloodBankSystemClass ui;
};

