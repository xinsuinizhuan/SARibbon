﻿#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextEdit>
#include <QAbstractButton>
#include <QFileDialog>
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include <QPushButton>
#include <QMessageBox>
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include <QDebug>
#include <QElapsedTimer>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSpinBox>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QFontComboBox>
#include <QLabel>
#include "SARibbonMenu.h"
#include "SARibbonComboBox.h"
#include "SARibbonLineEdit.h"
#include "SARibbonGallery.h"
#include "SARibbonCheckBox.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonApplicationButton.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonElementManager.h"
#include "SARibbonCustomizeDialog.h"
#include "SARibbonColorToolButton.h"
#include "SAFramelessHelper.h"
#include "colorWidgets/SAColorPaletteGridWidget.h"
#include "colorWidgets/SAColorGridWidget.h"
#define PRINT_COST_START()                                                                                             \
    QElapsedTimer __TMP_COST;                                                                                          \
    __TMP_COST.start();                                                                                                \
    int __TMP_LASTTIMES = 0

#define PRINT_COST(STR)                                                                                                \
    do {                                                                                                               \
        int ___TMP_INT = __TMP_COST.elapsed();                                                                         \
        qDebug() << STR << " cost " << ___TMP_INT - __TMP_LASTTIMES << " ms (" << ___TMP_INT << ")";                   \
        m_edit->append(QString("%1 cost %2 ms(%3)").arg(STR).arg(___TMP_INT - __TMP_LASTTIMES).arg(___TMP_INT));       \
        __TMP_LASTTIMES = ___TMP_INT;                                                                                  \
    } while (0)

MainWindow::MainWindow(QWidget* par) : SARibbonMainWindow(par), m_customizeWidget(nullptr), m_applicationMenu(nullptr)
{
    PRINT_COST_START();
    SAFramelessHelper* helper = framelessHelper();
    helper->setRubberBandOnResize(false);
    setWindowTitle(("ribbon mainwindow test"));
    m_edit = new QTextEdit(this);
    setCentralWidget(m_edit);
    setStatusBar(new QStatusBar());

    SARibbonBar* ribbon = ribbonBar();
    //通过setContentsMargins设置ribbon四周的间距
    ribbon->setContentsMargins(5, 0, 5, 0);
    //设置applicationButton
    PRINT_COST("setCentralWidget & setWindowTitle");
    ribbon->applicationButton()->setText(("   File   "));  //文字两边留有间距，好看一点

    //! 创建application菜单
    createRibbonApplicationMenu();

    //! 添加主标签页
    //! 这里演示了通过addCategoryPage工厂函数添加
    SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("Main"));
    categoryMain->setObjectName(("categoryMain"));
    createCategoryMain(categoryMain);
    PRINT_COST("new main page");

    //添加其他标签页 - 直接new SARibbonCategory添加
    SARibbonCategory* categoryOther = new SARibbonCategory();
    categoryOther->setCategoryName(tr("Other"));
    categoryOther->setObjectName(("categoryOther"));
    createCategoryOther(categoryOther);
    ribbon->addCategoryPage(categoryOther);
    PRINT_COST("add other page");

    //添加删除标签页
    SARibbonCategory* categoryDelete = new SARibbonCategory();
    categoryDelete->setCategoryName(("Delete"));
    categoryDelete->setObjectName(("categoryDelete"));
    ribbon->addCategoryPage(categoryDelete);
    createCategoryDelete(categoryDelete);
    PRINT_COST("add category delete page");

    //添加尺寸标签页
    SARibbonCategory* categorySize = new SARibbonCategory();
    categorySize->setCategoryName(("Size(example long category)"));
    categorySize->setObjectName(("categorySize"));
    ribbon->addCategoryPage(categorySize);
    createCategorySize(categorySize);
    PRINT_COST("add category size page");

    //添加颜色标签页
    SARibbonCategory* categoryColor = new SARibbonCategory();
    categoryColor->setCategoryName(("Color"));
    categoryColor->setObjectName(("categoryColor"));
    ribbon->addCategoryPage(categoryColor);
    createCategoryColor(categoryColor);
    PRINT_COST("add category color page");

    createContextCategory1();
    PRINT_COST("add context1 category page");
    createContextCategory2();
    PRINT_COST("add context2 category page");
    SARibbonQuickAccessBar* quickAccessBar = ribbon->quickAccessBar();
    createQuickAccessBar(quickAccessBar);
    PRINT_COST("add quick access bar");
    SARibbonButtonGroupWidget* rightBar = ribbon->rightButtonGroup();
    createRightButtonGroup(rightBar);
    PRINT_COST("add right bar");
    addSomeOtherAction();

    setMinimumWidth(500);
    //
    showMaximized();
    onStyleClicked(SARibbonBar::OfficeStyle);
    //
    setWindowIcon(QIcon(":/icon/icon/SA.svg"));
}

void MainWindow::createRibbonApplicationMenu()
{
    //以下和一个传统的QMenu操作是一样的
    if (!m_applicationMenu) {
        m_applicationMenu = new SARibbonMenu(this);
        m_applicationMenu->addAction(createAction("test1", ":/icon/icon/action.svg"));
        m_applicationMenu->addAction(createAction("test2", ":/icon/icon/action2.svg"));
        m_applicationMenu->addAction(createAction("test3", ":/icon/icon/action3.svg"));
        m_applicationMenu->addAction(createAction("test4", ":/icon/icon/action4.svg"));
    }
    // ribbonbar默认会创建一个SARibbonApplicationButton,当然用户也可以自定义button设置进去，例如定制一个圆形的菜单
    QAbstractButton* btn              = ribbonBar()->applicationButton();
    SARibbonApplicationButton* appBtn = qobject_cast< SARibbonApplicationButton* >(btn);
    if (!appBtn) {
        return;
    }
    appBtn->setMenu(m_applicationMenu);
}

void MainWindow::onShowContextCategory(bool on)
{
    if (m_contextCategory == nullptr) {
        createContextCategory1();
    }
    if (on) {
        this->ribbonBar()->showContextCategory(m_contextCategory);
    } else {
        this->ribbonBar()->hideContextCategory(m_contextCategory);
    }
}

void MainWindow::onStyleClicked(int id)
{

    SARibbonBar::RibbonStyle ribbonStyle = static_cast< SARibbonBar::RibbonStyle >(id);
    ribbonBar()->setRibbonStyle(ribbonStyle);
    mActionWordWrap->setChecked(SARibbonToolButton::isEnableWordWrap());
    switch (ribbonStyle) {
    case SARibbonBar::OfficeStyle:
        m_edit->append(
                tr("\nchange ribbon style to office style,The standard office style text display is line wrapped, "
                   "and you can also control whether it wrap through SARibbonToolButton::setEnableWordWrap"));  // cn:标准的office样式的文字显示是换行的，你也可以通过SARibbonToolButton::setEnableWordWrap来控制它是否换行
        m_edit->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::OfficeStyle);"));
        break;
    case SARibbonBar::OfficeStyleTwoRow:
        m_edit->append(tr("\nchange ribbon style to office style 2 row,All text in 2-line mode does not wrap, and you "
                          "can also control whether it wraps through SARibbonToolButton: setEnableWordWrap"));  // cn:所有2行模式的文字都是不换行的，你也可以通过SARibbonToolButton::setEnableWordWrap来控制它是否换行
        m_edit->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::OfficeStyleTwoRow);"));
        break;
    case SARibbonBar::WpsLiteStyle:
        m_edit->append(tr("\nchange ribbon style to wps style,The standard wps style text display is line wrapped, "
                          "and you can also control whether it wrap through SARibbonToolButton::setEnableWordWrap"));  // cn:标准的wps样式的文字显示是换行的，你也可以通过SARibbonToolButton::setEnableWordWrap来控制它是否换行
        m_edit->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::OfficeStyleTwoRow);"));
        break;
    case SARibbonBar::WpsLiteStyleTwoRow:
        m_edit->append(tr("\nchange ribbon style to wps style 2 row,All text in 2-line mode does not wrap, and you "
                          "can also control whether it wraps through SARibbonToolButton: setEnableWordWrap"));  // cn:所有2行模式的文字都是不换行的，你也可以通过SARibbonToolButton::setEnableWordWrap来控制它是否换行
        m_edit->append(tr("ribbonBar()->setRibbonStyle(SARibbonBar::OfficeStyleTwoRow);"));
        break;
    default:
        break;
    }
}

void MainWindow::onActionCustomizeTriggered(bool b)
{
    Q_UNUSED(b);
    if (nullptr == m_customizeWidget) {
        m_customizeWidget = new SARibbonCustomizeWidget(this, this, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint | Qt::Dialog);
        m_customizeWidget->setWindowModality(Qt::ApplicationModal);  //设置阻塞类型
        m_customizeWidget->setAttribute(Qt::WA_ShowModal, true);     //属性设置 true:模态 false:非模态
        m_customizeWidget->setupActionsManager(m_actMgr);
    }
    m_customizeWidget->show();
    m_customizeWidget->applys();
}

void MainWindow::onActionCustomizeAndSaveTriggered(bool b)
{
    Q_UNUSED(b);
    SARibbonCustomizeDialog dlg(this);
    dlg.setupActionsManager(m_actMgr);
    dlg.fromXml("customize.xml");
    if (SARibbonCustomizeDialog::Accepted == dlg.exec()) {
        dlg.applys();
        QByteArray str;
        QXmlStreamWriter xml(&str);
        xml.setAutoFormatting(true);
        xml.setAutoFormattingIndent(2);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)  // QXmlStreamWriter always encodes XML in UTF-8.
        xml.setCodec("utf-8");
#endif
        xml.writeStartDocument();
        bool isok = dlg.toXml(&xml);
        xml.writeEndDocument();
        if (isok) {
            QFile f("customize.xml");
            if (f.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
                QTextStream s(&f);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)  // QTextStream always encodes XML in UTF-8.
                s.setCodec("utf-8");
#endif
                s << str;
                s.flush();
            }
            m_edit->append("write xml:");
            m_edit->append(str);
        }
    }
}

void MainWindow::onActionHelpTriggered()
{
    QMessageBox::information(this,
                             tr("infomation"),
                             tr("\n ==============="
                                "\n SARibbonBar version:%1"
                                "\n Author:czy"
                                "\n Email:czy.t@163.com"
                                "\n ===============")
                                     .arg(SARibbonBar::versionString()));
}

void MainWindow::onActionRemoveAppBtnTriggered(bool b)
{
    if (b) {
        ribbonBar()->setApplicationButton(nullptr);
    } else {
        SARibbonApplicationButton* actionRemoveAppBtn = new SARibbonApplicationButton();
        actionRemoveAppBtn->setText(tr("   File   "));
        this->ribbonBar()->setApplicationButton(actionRemoveAppBtn);
        createRibbonApplicationMenu();
    }
}

void MainWindow::onActionUseQssTriggered()
{
    QFile f("ribbon.qss");
    if (!f.exists()) {
        QString fdir = QFileDialog::getOpenFileName(this, tr("select qss file"));
        if (fdir.isEmpty()) {
            return;
        }
        f.setFileName(fdir);
    }
    if (!f.open(QIODevice::ReadWrite)) {
        return;
    }
    QString qss(f.readAll());
    m_edit->setText(qss);
    this->ribbonBar()->setStyleSheet(qss);
}

void MainWindow::onActionLoadCustomizeXmlFileTriggered()
{
    //只能调用一次
    static bool has_call = false;
    if (!has_call) {
        has_call = sa_apply_customize_from_xml_file("customize.xml", this, m_actMgr);
    }
}

void MainWindow::onActionWindowFlagNormalButtonTriggered(bool b)
{
    if (b) {
        //最大最小关闭按钮都有
        Qt::WindowFlags f = windowFlags();
        f |= (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
        updateWindowFlag(f);
    } else {
        //由于已经处于frameless状态，这个最大最小设置是无效的
        // setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
        Qt::WindowFlags f = windowFlags();
        f &= ~Qt::WindowMinMaxButtonsHint & ~Qt::WindowCloseButtonHint;
        updateWindowFlag(f);
    }
}

void MainWindow::onFontComWidgetCurrentFontChanged(const QFont& f)
{
    qDebug() << "set font:" << f;
    ribbonBar()->setFont(f);
    update();
}

void MainWindow::onActionFontLargerTriggered()
{
    QFont f = font();
    f.setPointSize(f.pointSize() + 1);
    ribbonBar()->setFont(f);
    update();
    qDebug() << "set font:" << f;
}

void MainWindow::onActionFontSmallerTriggered()
{
    QFont f = font();
    f.setPointSize(f.pointSize() - 1);
    ribbonBar()->setFont(f);
    update();
    qDebug() << "set font:" << f;
}

void MainWindow::onActionWordWrapTriggered(bool b)
{
    ribbonBar()->setEnableWordWrap(b);
    m_edit->append(tr("By using the SARibbonBar::setEnableWordWrap function, "
                      "you can set whether text breaks or not.\n"
                      "By default, the two line mode will not wrap, the three line mode will wrap.\n"
                      "You can force the two line mode to wrap, or the three line mode to not wrap"));
    // cn:通过SARibbonBar::setEnableWordWrap函数可以设置文字是否换行。\n
    // 默认情况下，两行模式都不会换行，三行模式下都会换行。\n
    // 可以强制设置两行模式也换行，或者三行模式不换行
}

/**
 * @brief 测试SARibbonButtonGroupWidget和标题对齐
 * @param act
 */
void MainWindow::onButtonGroupActionTriggered(QAction* act)
{
    QVariant v = act->property("align");
    if (v.isValid()) {
        Qt::Alignment al = static_cast< Qt::Alignment >(v.toInt());
        if (!ribbonBar()) {
            return;
        }
        ribbonBar()->setWindowTitleAligment(al);
        ribbonBar()->repaint();
    }
}

/**
 * @brief 颜色按钮点击
 * @param c
 * @param on
 */
void MainWindow::onColorButtonColorClicked(const QColor& c, bool on)
{
    Q_UNUSED(on);
    m_edit->append(QString("color click %1").arg(c.name()));
}

void MainWindow::createCategoryMain(SARibbonCategory* page)
{
    //! 1
    //! pannel 1 start
    //!

    //使用addPannel函数来创建SARibbonPannel，效果和new SARibbonPannel再addPannel一样
    SARibbonPannel* pannelStyle = page->addPannel(("ribbon style"));

    QAction* actSave = createAction(tr("Save"), ":/icon/icon/save.svg");
    actSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
    pannelStyle->addLargeAction(actSave);
    connect(actSave, &QAction::triggered, this, [ this ](bool b) {
        Q_UNUSED(b);
        this->m_edit->append("actSaveion clicked");
    });

    QAction* actHideRibbon = createAction(tr("hide ribbon"), ":/icon/icon/hideRibbon.svg", "actHideRibbon");
    actHideRibbon->setCheckable(true);
    pannelStyle->addSmallAction(actHideRibbon);
    connect(actHideRibbon, &QAction::triggered, this, [ this ](bool b) { this->ribbonBar()->setMinimumMode(b); });
    connect(ribbonBar(), &SARibbonBar::ribbonModeChanged, this, [ actHideRibbon ](SARibbonBar::RibbonMode nowNode) {
        actHideRibbon->setChecked(nowNode == SARibbonBar::MinimumRibbonMode);
    });

    QAction* actShowHideButton = createAction(tr("show \nhide button"), ":/icon/icon/showHideButton.svg", "show hide button");
    actShowHideButton->setCheckable(true);
    pannelStyle->addSmallAction(actShowHideButton);  // wrod wrap was not effect in small button
    connect(actShowHideButton, &QAction::triggered, this, [ this ](bool b) {
        this->ribbonBar()->showMinimumModeButton(b);  //显示ribbon最小化按钮
    });
    actShowHideButton->trigger();

    mActionWordWrap = createAction(tr("word wrap"), ":/icon/icon/wordwrap.svg");
    mActionWordWrap->setCheckable(ribbonBar()->isEnableWordWrap());
    pannelStyle->addSmallAction(mActionWordWrap);
    connect(mActionWordWrap, &QAction::triggered, this, &MainWindow::onActionWordWrapTriggered);

    QButtonGroup* g = new QButtonGroup(page);

    QRadioButton* r = new QRadioButton();
    r->setText(tr("use office style"));
    r->setObjectName(("use office style"));
    r->setChecked(true);
    pannelStyle->addSmallWidget(r);
    g->addButton(r, SARibbonBar::OfficeStyle);

    r = new QRadioButton();
    r->setObjectName(("use wps style"));
    r->setText(tr("use wps style"));
    r->setChecked(false);
    pannelStyle->addSmallWidget(r);
    g->addButton(r, SARibbonBar::WpsLiteStyle);

    r = new QRadioButton();
    r->setObjectName(("use office 2row style"));
    r->setText(tr("use office 2 row style"));
    r->setChecked(false);
    pannelStyle->addSmallWidget(r);
    g->addButton(r, SARibbonBar::OfficeStyleTwoRow);

    r = new QRadioButton();
    r->setObjectName(("use wps 2row style"));
    r->setText(tr("use wps 2row style"));
    r->setChecked(false);
    pannelStyle->addSmallWidget(r);
    g->addButton(r, SARibbonBar::WpsLiteStyleTwoRow);

//    connect(g, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &MainWindow::onStyleClicked);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(g, static_cast< void (QButtonGroup::*)(int) >(&QButtonGroup::idClicked), this, &MainWindow::onStyleClicked);
#else
    connect(g, static_cast< void (QButtonGroup::*)(int) >(&QButtonGroup::buttonClicked), this, &MainWindow::onStyleClicked);
#endif

    SARibbonPannel* pannelToolButtonStyle = page->addPannel(("sa ribbon toolbutton style"));

    SARibbonToolButton* btn;
    SARibbonMenu* menu = new SARibbonMenu(this);
    QAction* a         = nullptr;
    {
        QIcon itemicon = QIcon(":/icon/icon/item.svg");
        for (int i = 0; i < 5; ++i) {
            a = menu->addAction(itemicon, tr("item %1").arg(i + 1));
            a->setObjectName(QStringLiteral("item %1").arg(i + 1));
        }
    }

    QAction* act = createAction(tr("test 1"), ":/icon/icon/test1.svg");
    act->setMenu(menu);
    act->setToolTip(tr("use QToolButton::MenuButtonPopup mode"));
    btn = pannelToolButtonStyle->addSmallAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);

    act = createAction(tr("test 2"), ":/icon/icon/test2.svg");
    act->setMenu(menu);
    act->setToolTip(tr("use QToolButton::InstantPopup mode"));
    btn = pannelToolButtonStyle->addSmallAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);

    pannelToolButtonStyle->addSeparator();

    act = createAction(tr("Delayed\nPopup"), ":/icon/icon/folder-cog.svg");
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);

    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckabletriggered);

    act = createAction(tr("Menu Button Popup"), ":/icon/icon/folder-star.svg");
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckabletriggered);

    act = createAction(tr("Instant Popup"), ":/icon/icon/folder-stats.svg");
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::InstantPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onInstantPopupCheckabletriggered);

    act = createAction(tr("Delayed Popup checkable"), ":/icon/icon/folder-table.svg");
    act->setCheckable(true);
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::DelayedPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onDelayedPopupCheckableTest);

    act = createAction(tr("Menu Button Popup checkable"), ":/icon/icon/folder-checkmark.svg");
    act->setCheckable(true);
    act->setMenu(menu);
    btn = pannelToolButtonStyle->addLargeAction(act);
    btn->setPopupMode(QToolButton::MenuButtonPopup);
    connect(act, &QAction::triggered, this, &MainWindow::onMenuButtonPopupCheckableTest);

    act = createAction(tr("disable action"), ":/icon/icon/disable.svg");
    act->setCheckable(true);
    act->setMenu(menu);
    act->setEnabled(false);
    pannelToolButtonStyle->addLargeAction(act);

    QAction* optAct = new QAction(this);
    connect(optAct, &QAction::triggered, this, [ this ](bool on) {
        Q_UNUSED(on);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered"));
    });
    pannelToolButtonStyle->addOptionAction(optAct);
    //! 2
    //! pannel 2 start
    //!
    SARibbonPannel* pannel2 = page->addPannel(("pannel 2"));

    QAction* actShowContext = createAction(tr("show Context"), ":/icon/icon/showContext.svg");
    actShowContext->setCheckable(true);
    pannel2->addLargeAction(actShowContext);
    connect(actShowContext, &QAction::triggered, this, &MainWindow::onShowContextCategory);

    QAction* actDeleteContext = createAction(tr("delete Context"), ":/icon/icon/deleteContext.svg");
    pannel2->addLargeAction(actDeleteContext);
    connect(actDeleteContext, &QAction::triggered, this, [ this, act ](bool on) {
        Q_UNUSED(on);
        if (this->m_contextCategory) {
            this->ribbonBar()->destroyContextCategory(this->m_contextCategory);
            this->m_contextCategory = nullptr;
            act->setDisabled(true);
        }
    });

    act = createAction(tr("Word\nWrap"), ":/icon/icon/setText.svg");
    pannel2->addLargeAction(act);
    connect(act, &QAction::triggered, this, [ this ](bool on) {
        Q_UNUSED(on);
        this->m_edit->append(tr("Text can be manually wrapped(use \\n), and will appear as 1 line in the case of "
                                "SARibbonBar::setEnableWordWrap (false)"));  // cn:文本中手动换行
    });

    act = createAction(tr("Word\nWrap"), ":/icon/icon/setText.svg");
    act->setMenu(menu);
    pannel2->addLargeAction(act);
    connect(act, &QAction::triggered, this, [ this ](bool on) {
        Q_UNUSED(on);
        this->m_edit->append(tr("Text can be manually wrapped(use \\n), and will appear as 1 line in the case of "
                                "SARibbonBar::setEnableWordWrap (false)"));  // cn:文本中手动换行
    });
    //! 3
    //! pannel 3 start -> widget test
    //!

    SARibbonPannel* pannelWidgetTest = page->addPannel(tr("widget test"));
    pannelWidgetTest->setObjectName(QStringLiteral(u"pannelWidgetTest"));

    SARibbonComboBox* com = new SARibbonComboBox(this);

    com->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    com->setWindowTitle(tr("ComboBox"));
    for (int i = 0; i < 40; ++i) {
        com->addItem(QString("SARibbonComboBox test%1").arg(i + 1));
    }
    com->setEditable(true);
    pannelWidgetTest->addSmallWidget(com);

    com = new SARibbonComboBox(this);
    com->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    com->setWindowTitle("ComboBox Editable");
    for (int i = 0; i < 40; ++i) {
        com->addItem(QString("item %1").arg(i + 1));
    }
    pannelWidgetTest->addSmallWidget(com);

    SARibbonLineEdit* lineEdit = new SARibbonLineEdit(this);

    lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    lineEdit->setWindowTitle("Line Edit");
    lineEdit->setText("SARibbonLineEdit");
    pannelWidgetTest->addSmallWidget(lineEdit);
    QWidget* w = lineEdit->parentWidget();

    while (w) {
        qDebug() << w->metaObject()->className();
        w = w->parentWidget();
    }
    SARibbonCheckBox* checkBox = new SARibbonCheckBox(this);

    checkBox->setText(tr("checkBox"));
    pannelWidgetTest->addSmallWidget(checkBox);
    pannelWidgetTest->addSeparator();

    QCalendarWidget* calendarWidget = new QCalendarWidget(this);
    calendarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    calendarWidget->setObjectName(("calendarWidget"));
    pannelWidgetTest->addLargeWidget(calendarWidget);
    pannelWidgetTest->setExpanding();
    optAct = new QAction(this);
    connect(optAct, &QAction::triggered, this, [ this ](bool on) {
        Q_UNUSED(on);
        QMessageBox::information(this, tr("Option Action Triggered"), tr("Option Action Triggered"));
    });
    pannelWidgetTest->addOptionAction(optAct);

    pannelWidgetTest->setVisible(true);
}

void MainWindow::createCategoryOther(SARibbonCategory* page)
{

    SARibbonPannel* pannel1 = new SARibbonPannel(tr("pannel one"));
    pannel1->setObjectName("CategoryOther-pannel1");
    page->addPannel(pannel1);
    //按钮组
    SARibbonButtonGroupWidget* btnGroup1 = new SARibbonButtonGroupWidget(pannel1);
    btnGroup1->addAction(createAction(tr("Decrease Margin"), ":/icon/icon/Decrease-Margin.svg"));
    btnGroup1->addAction(createAction(tr("Decrease Indent"), ":/icon/icon/Decrease-Indent.svg"));
    btnGroup1->addAction(createAction(tr("Wrap Image Left"), ":/icon/icon/Wrap-Image Left.svg"));
    btnGroup1->addAction(createAction(tr("Wrap Image Right"), ":/icon/icon/Wrap-Image Right.svg"));
    pannel1->addWidget(btnGroup1, SARibbonPannelItem::Medium);
    SARibbonButtonGroupWidget* btnGroup2 = new SARibbonButtonGroupWidget(pannel1);
    QAction* titleAlgnment               = createAction(tr("Align Right"), ":/icon/icon/Align-Right.svg");
    titleAlgnment->setProperty("align", (int)Qt::AlignRight | Qt::AlignVCenter);
    btnGroup2->addAction(titleAlgnment);
    titleAlgnment = createAction(tr("Align Left"), ":/icon/icon/Align-Left.svg");
    titleAlgnment->setProperty("align", (int)Qt::AlignLeft | Qt::AlignVCenter);
    btnGroup2->addAction(titleAlgnment);
    titleAlgnment = createAction(tr("Align Center"), ":/icon/icon/Align-Center.svg");
    titleAlgnment->setProperty("align", (int)Qt::AlignCenter);
    btnGroup2->addAction(titleAlgnment);
    pannel1->addWidget(btnGroup2, SARibbonPannelItem::Medium);
    connect(btnGroup2, &SARibbonButtonGroupWidget::actionTriggered, this, &MainWindow::onButtonGroupActionTriggered);
    // Gallery
    SARibbonGallery* gallery = pannel1->addGallery();
    QList< QAction* > galleryActions;
    auto lambdaCreateGalleryAction = [ this ](const QString& text, const QString& iconurl) -> QAction* {
        QAction* act = this->createAction(text, iconurl);
        this->connect(act, &QAction::triggered, this, [ this, text ]() {
            if (this->m_edit) {
                this->m_edit->append(QString("%1 triggered").arg(text));
            }
        });
        return act;
    };
    galleryActions.append(lambdaCreateGalleryAction("Document File", ":/gallery-icon/icon/gallery/Document-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Download File", ":/gallery-icon/icon/gallery/Download-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Drive File Four Word", ":/gallery-icon/icon/gallery/Drive-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Dropbox File", ":/gallery-icon/icon/gallery/Dropbox-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Email File", ":/gallery-icon/icon/gallery/Email-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Encode File", ":/gallery-icon/icon/gallery/Encode-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Favorit File", ":/gallery-icon/icon/gallery/Favorit-File.svg"));
    galleryActions.append(lambdaCreateGalleryAction("File Error", ":/gallery-icon/icon/gallery/File-Error.svg"));
    galleryActions.append(lambdaCreateGalleryAction("File Read Only", ":/gallery-icon/icon/gallery/File-Readonly.svg"));
    galleryActions.append(lambdaCreateGalleryAction("File Settings", ":/gallery-icon/icon/gallery/File-Settings.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Presentation File", ":/gallery-icon/icon/gallery/Presentation-File.svg"));
    SARibbonGalleryGroup* group1 = gallery->addCategoryActions(tr("Files"), galleryActions);
    group1->setGalleryGroupStyle(SARibbonGalleryGroup::IconWithWordWrapText);
    group1->setGridMinimumWidth(80);
    galleryActions.clear();
    galleryActions.append(lambdaCreateGalleryAction("Photoshop", ":/gallery-icon/icon/gallery/Photoshop.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Internet-Explorer", ":/gallery-icon/icon/gallery/Internet-Explorer.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Illustrator", ":/gallery-icon/icon/gallery/Illustrator.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Google-Maps", ":/gallery-icon/icon/gallery/Google-Maps.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Adobe", ":/gallery-icon/icon/gallery/Adobe.svg"));
    galleryActions.append(lambdaCreateGalleryAction("Word", ":/gallery-icon/icon/gallery/Word.svg"));
    SARibbonGalleryGroup* group2 = gallery->addCategoryActions(tr("Apps"), galleryActions);
    group2->setGridMinimumWidth(80);
    gallery->setCurrentViewGroup(group1);

    QAction* optAct = new QAction(this);

    optAct->setObjectName(("debug"));
    pannel1->addOptionAction(optAct);

    SARibbonPannel* pannel2 = new SARibbonPannel(tr("pannel two"));
    pannel2->setObjectName("CategoryOther-pannel2");
    page->addPannel(pannel2);
    QAction* actionRemoveAppBtn = createAction(tr("remove application button"), ":/icon/icon/remove-app-btn.svg");
    actionRemoveAppBtn->setCheckable(true);
    connect(actionRemoveAppBtn, &QAction::toggled, this, &MainWindow::onActionRemoveAppBtnTriggered);
    pannel2->addLargeAction(actionRemoveAppBtn);

    QAction* actionLongText = createAction(tr("show very long text in a button,balabalabala etc"), ":/icon/icon/long-text.svg", "long-text");
    pannel2->addLargeAction(actionLongText);

    SARibbonPannel* pannelStyle = new SARibbonPannel(tr("style"));
    pannelStyle->setObjectName("CategoryOther-pannelStyle");
    page->addPannel(pannelStyle);

    QAction* actionUseQss = createAction(tr("use qss"), ":/icon/icon/useqss.svg");
    connect(actionUseQss, &QAction::triggered, this, &MainWindow::onActionUseQssTriggered);
    pannelStyle->addSmallAction(actionUseQss);

    QAction* actionLoadCustomizeXmlFile = createAction(tr("load customize from xml file"), ":/icon/icon/useCustomize.svg");
    connect(actionLoadCustomizeXmlFile, &QAction::triggered, this, &MainWindow::onActionLoadCustomizeXmlFileTriggered);
    pannelStyle->addSmallAction(actionLoadCustomizeXmlFile);

    QAction* actionWindowFlagNormalButton = createAction(tr("window normal button"), ":/icon/icon/windowsflag-normal.svg");
    actionWindowFlagNormalButton->setCheckable(true);
    actionWindowFlagNormalButton->setChecked(true);
    pannelStyle->addSmallAction(actionWindowFlagNormalButton);
    connect(actionWindowFlagNormalButton, &QAction::triggered, this, &MainWindow::onActionWindowFlagNormalButtonTriggered);

    SARibbonPannel* pannelUtf8 = new SARibbonPannel(QStringLiteral(u"中文显示测试"));
    pannelUtf8->setObjectName("CategoryOther-pannelUtf8");
    page->addPannel(pannelUtf8);

    QAction* actionChangeText = createAction(QStringLiteral(u"改变显示为英文"), ":/icon/icon/chinese-char.svg", "actionChangeText");
    actionChangeText->setCheckable(true);
    actionChangeText->setChecked(false);
    pannelUtf8->addLargeAction(actionChangeText);
    connect(actionChangeText, &QAction::triggered, this, [ pannelUtf8, actionChangeText ](bool on) {
        if (on) {
            pannelUtf8->setPannelName(QStringLiteral(u"show chinese char"));
            actionChangeText->setText(QStringLiteral(u"show in chinese"));
        } else {
            pannelUtf8->setPannelName(QStringLiteral(u"中文显示测试"));
            actionChangeText->setText(QStringLiteral(u"改变显示为英文"));
        }
    });

    SARibbonPannel* pannelContextCategory = new SARibbonPannel(("Context Category"));
    page->addPannel(pannelContextCategory);
    QAction* a1 = createAction("Context Category 1", ":/icon/icon/ContextCategory.svg");
    a1->setCheckable(true);
    connect(a1, &QAction::triggered, this, [ this ](bool c) {
        this->ribbonBar()->setContextCategoryVisible(m_contextCategory, c);
    });

    QAction* a2 = createAction("Context Category 2", ":/icon/icon/ContextCategory.svg");
    a2->setCheckable(true);
    connect(a2, &QAction::triggered, this, [ this ](bool c) {
        this->ribbonBar()->setContextCategoryVisible(m_contextCategory2, c);
    });
    pannelContextCategory->addLargeAction(a1);
    pannelContextCategory->addLargeAction(a2);
}

void MainWindow::createCategoryDelete(SARibbonCategory* page)
{
    SARibbonPannel* pannel1 = new SARibbonPannel(("pannel 1"));
    SARibbonPannel* pannel2 = new SARibbonPannel(("pannel 2"));

    QAction* actionRemovePanne = createAction(tr("remove pannel"), ":/icon/icon/remove.svg");

    connect(actionRemovePanne, &QAction::triggered, this, [ page, pannel2 ]() { page->removePannel(pannel2); });
    pannel1->addLargeAction(actionRemovePanne);

    QAction* actionRemoveCategory = createAction(tr("remove this category"), ":/icon/icon/removePage.svg");

    connect(actionRemoveCategory, &QAction::triggered, this, [ this, page, actionRemoveCategory ]() {
        this->ribbonBar()->removeCategory(page);
        page->hide();
        page->deleteLater();
        actionRemoveCategory->setDisabled(true);
    });
    pannel2->addLargeAction(actionRemoveCategory);

    page->addPannel(pannel1);
    page->addPannel(pannel2);
}

/**
 * @brief 构建尺寸页
 * @param page
 */
void MainWindow::createCategorySize(SARibbonCategory* page)
{
    QAction* act                      = nullptr;
    SARibbonPannel* pannel            = page->addPannel(tr("Font"));
    SARibbonButtonGroupWidget* group1 = new SARibbonButtonGroupWidget(pannel);
    group1->setObjectName(QStringLiteral(u"group1"));
    QLabel* labelFontSize = new QLabel(group1);
    labelFontSize->setText(tr("select font"));
    labelFontSize->setObjectName(QStringLiteral(u"labelFontSize"));
    group1->addWidget(labelFontSize);
    QFontComboBox* fontComWidget = new QFontComboBox(group1);
    fontComWidget->setObjectName(QStringLiteral(u"fontComboBox"));
    connect(fontComWidget, &QFontComboBox::currentFontChanged, this, &MainWindow::onFontComWidgetCurrentFontChanged);
    group1->addWidget(fontComWidget);
    act = pannel->addWidget(group1, SARibbonPannelItem::Medium);
    act->setObjectName(labelFontSize->objectName());

    SARibbonButtonGroupWidget* group2 = new SARibbonButtonGroupWidget(pannel);
    group2->setObjectName(QStringLiteral(u"group2"));
    group2->addAction(createAction("Bold", ":/icon/icon/bold.svg"));
    group2->addAction(createAction("Italic", ":/icon/icon/Italic.svg"));
    group2->addSeparator();
    group2->addAction(createAction("left alignment", ":/icon/icon/al-left.svg"));
    group2->addAction(createAction("center alignment", ":/icon/icon/al-center.svg"));
    group2->addAction(createAction("right alignment", ":/icon/icon/al-right.svg"));
    group2->addAction(createAction("line up on both sides", ":/icon/icon/al-bothside.svg"));
    act = pannel->addWidget(group2, SARibbonPannelItem::Medium);
    act->setObjectName(group2->objectName());

    pannel->addSeparator();

    QAction* actLargerFontSize = createAction(tr("Larger"), ":/icon/icon/largerFont.svg", "actLargerFontSize");
    pannel->addLargeAction(actLargerFontSize);

    QAction* actSmallFontSize = createAction(tr("Smaller"), ":/icon/icon/smallFont.svg", "actSmallFontSize");
    pannel->addLargeAction(actSmallFontSize);

    connect(actLargerFontSize, &QAction::triggered, this, &MainWindow::onActionFontLargerTriggered);
    connect(actSmallFontSize, &QAction::triggered, this, &MainWindow::onActionFontSmallerTriggered);
}

void MainWindow::createCategoryColor(SARibbonCategory* page)
{
    SARibbonPannel* pannel = page->addPannel(tr("color"));

    auto fpCreateBtn = [ this, pannel ](const QColor& defaultColor = Qt::red) -> SARibbonColorToolButton* {
        SARibbonColorToolButton* colorButton = new SARibbonColorToolButton(pannel);
        colorButton->setColor(defaultColor);
        colorButton->setupStandardColorMenu();
        this->connect(colorButton, &SARibbonColorToolButton::colorClicked, this, &MainWindow::onColorButtonColorClicked);
        return colorButton;
    };
    // No Icon No text
    SARibbonColorToolButton* colorButton = fpCreateBtn();
    colorButton->setObjectName("ColorFillToIcon-NoIconNoText");
    colorButton->setColorStyle(SARibbonColorToolButton::ColorFillToIcon);
    pannel->addSmallWidget(colorButton);

    // No Icon have text
    colorButton = fpCreateBtn(Qt::blue);
    colorButton->setColorStyle(SARibbonColorToolButton::ColorFillToIcon);
    colorButton->setObjectName("ColorFillToIcon-NoIconHaveText");
    colorButton->setText("No Icon have text");
    pannel->addSmallWidget(colorButton);

    // have Icon No text
    colorButton = fpCreateBtn(QColor());
    colorButton->setIcon(QIcon(":/icon/icon/long-text.svg"));
    pannel->addSmallWidget(colorButton);

    // have Icon have text
    colorButton = fpCreateBtn(Qt::red);
    colorButton->setIcon(QIcon(":/icon/icon/long-text.svg"));
    colorButton->setText("have Icon have text");
    pannel->addSmallWidget(colorButton);

    colorButton = fpCreateBtn(QColor());
    colorButton->setButtonType(SARibbonToolButton::LargeButton);
    colorButton->setObjectName("ColorFillToIcon-LargeColorButton");
    colorButton->setColorStyle(SARibbonColorToolButton::ColorFillToIcon);
    colorButton->setText("Large Color Button");
    pannel->addLargeWidget(colorButton);

    colorButton = fpCreateBtn(QColor());
    colorButton->setButtonType(SARibbonToolButton::LargeButton);
    colorButton->setIcon(QIcon(":/icon/icon/long-text.svg"));
    colorButton->setObjectName("ColorUnderIcon-LargeColorButton");
    colorButton->setText("Large Color Button");
    pannel->addLargeWidget(colorButton);
    //
    pannel->addSeparator();
    SAColorPaletteGridWidget* pw = new SAColorPaletteGridWidget(SA::getStandardColorList(), pannel);
    pw->setFactor({ 75, 120 });
    pannel->addLargeWidget(pw);
}

/**
 * @brief 创建上下文标签
 */
void MainWindow::createContextCategory1()
{
    //上下文标签
    SARibbonBar* ribbon = ribbonBar();
    m_contextCategory   = ribbon->addContextCategory(tr("context"), QColor(), 1);

    SARibbonCategory* contextCategoryPage1 = m_contextCategory->addCategoryPage(tr("Page1"));
    createContextCategoryPage1(contextCategoryPage1);

    SARibbonCategory* contextCategoryPage2 = m_contextCategory->addCategoryPage(tr("Page2"));
    createContextCategoryPage2(contextCategoryPage2);
}

void MainWindow::createContextCategory2()
{
    SARibbonBar* ribbon = ribbonBar();
    m_contextCategory2  = ribbon->addContextCategory(("context2"), QColor(), 2);
    m_contextCategory2->addCategoryPage(("context2 Page1"));
    m_contextCategory2->addCategoryPage(("context2 Page2"));
}

void MainWindow::createContextCategoryPage1(SARibbonCategory* page)
{
    SARibbonPannel* pannel = page->addPannel(tr("show and hide test"));

    QAction* actionHidePannel = createAction("hide pannel", ":/icon/icon/hidePannel.svg");
    actionHidePannel->setCheckable(true);
    pannel->addLargeAction(actionHidePannel);

    QAction* actionDisable = createAction(tr("Disable"), ":/icon/icon/enableTest.svg");

    actionDisable->setDisabled(true);
    pannel->addLargeAction(actionDisable);
    connect(actionDisable, &QAction::triggered, this, [ actionDisable ](bool b) {
        Q_UNUSED(b);
        actionDisable->setDisabled(true);
    });

    QAction* actionUnlock = createAction(tr("unlock"), ":/icon/icon/unlock.svg");
    actionUnlock->setShortcut(QKeySequence(QLatin1String("Ctrl+E")));
    pannel->addLargeAction(actionUnlock);
    connect(actionUnlock, &QAction::triggered, this, [ actionDisable ](bool b) {
        Q_UNUSED(b);
        actionDisable->setEnabled(true);
        actionDisable->setText(("Enabled"));
    });

    QAction* actionSetTextTest = createAction("set text", ":/icon/icon/setText.svg");

    actionSetTextTest->setCheckable(true);
    actionSetTextTest->setShortcut(QKeySequence(QLatin1String("Ctrl+D")));
    pannel->addLargeAction(actionSetTextTest);

    connect(actionSetTextTest, &QAction::toggled, this, [ actionSetTextTest ](bool b) {
        if (b) {
            actionSetTextTest->setText(QStringLiteral(u"setText测试"));
        } else {
            actionSetTextTest->setText(QStringLiteral(u"set text"));
        }
    });
    //隐藏pannel
    QAction* actionShowTest = createAction("show beside pannel", ":/icon/icon/show.svg");

    actionShowTest->setCheckable(true);
    pannel->addLargeAction(actionShowTest);

    SARibbonPannel* pannel2 = page->addPannel(tr("show/hide"));

    pannel2->addLargeAction(actionSetTextTest);

    connect(actionShowTest, &QAction::toggled, this, [ actionShowTest, pannel2, this ](bool b) {
        pannel2->setVisible(!b);
        if (b) {
            actionShowTest->setText(tr("hide beside pannel"));
        } else {
            actionShowTest->setText(tr("show beside pannel"));
        }
        ribbonBar()->repaint();
    });

    SARibbonPannel* pannel3 = page->addPannel(("show/hide action test"));

    QAction* actionHideAction2 = createAction("hide action 2", ":/icon/icon/action.svg");
    QAction* actionHideAction4 = createAction("hide action 4", ":/icon/icon/action.svg");
    actionHideAction2->setCheckable(true);
    actionHideAction2->setChecked(true);
    actionHideAction4->setCheckable(true);
    actionHideAction4->setChecked(true);
    QAction* act2 = createAction("action2", ":/icon/icon/action2.svg");
    QAction* act3 = createAction("action3", ":/icon/icon/action3.svg");
    QAction* act4 = createAction("action4", ":/icon/icon/action4.svg");
    pannel3->addLargeAction(actionHideAction2);
    pannel3->addLargeAction(actionHideAction4);
    pannel3->addSmallAction(act2);
    pannel3->addSmallAction(act3);
    pannel3->addSmallAction(act4);
    connect(actionHideAction2, &QAction::triggered, this, [ actionHideAction2, act2 ](bool b) {
        if (b) {
            act2->setVisible(true);
            actionHideAction2->setText(tr("hide action2"));
        } else {
            act2->setVisible(false);
            actionHideAction2->setText(tr("show action2"));
        }
    });
    connect(actionHideAction4, &QAction::triggered, this, [ actionHideAction4, act4 ](bool b) {
        if (b) {
            act4->setVisible(true);
            actionHideAction4->setText(tr("hide action4"));
        } else {
            act4->setVisible(false);
            actionHideAction4->setText(tr("show action4"));
        }
    });
    //
    SARibbonPannel* pannel4               = page->addPannel(("widget"));
    QSpinBox* spinbox                     = new QSpinBox(this);
    SARibbonCtrlContainer* ctrlContainer1 = new SARibbonCtrlContainer(pannel4);
    ctrlContainer1->setContainerWidget(spinbox);
    ctrlContainer1->setText(tr("spinbox:"));
    ctrlContainer1->setEnableShowIcon(false);
    ctrlContainer1->setMaximumHeight(fontMetrics().lineSpacing() * 1.5);
    pannel4->addMediumWidget(ctrlContainer1);

    QLineEdit* linedit                    = new QLineEdit(this);
    SARibbonCtrlContainer* ctrlContainer2 = new SARibbonCtrlContainer(pannel4);
    ctrlContainer2->setContainerWidget(linedit);
    ctrlContainer2->setText(tr("linedit:"));
    ctrlContainer2->setEnableShowIcon(false);
    ctrlContainer2->setMaximumHeight(fontMetrics().lineSpacing() * 1.5);
    pannel4->addMediumWidget(ctrlContainer2);
}

void MainWindow::createContextCategoryPage2(SARibbonCategory* page)
{
    SARibbonPannel* pannel1 = page->addPannel(("删除CategoryPage测试"));

    QAction* actionDeleteThisCategory = createAction("delete this category", ":/icon/icon/delete.svg");
    pannel1->addLargeAction(actionDeleteThisCategory);
    connect(actionDeleteThisCategory, &QAction::triggered, this, [ this, page ]() {
        this->ribbonBar()->removeCategory(page);
        page->deleteLater();
    });
    SARibbonPannel* pannelLayout = page->addPannel(("特殊布局"));

    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large1"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small1"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small2"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addSeparator();
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small3"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small4"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small5"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small6"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addSeparator();
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large2"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium1"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium2"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small7"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium3"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large3"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium4"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium5"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large4"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addSeparator();
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium6"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Large", ":/icon/icon/layout.svg", "@Large5"), QToolButton::InstantPopup, SARibbonPannelItem::Large);
    pannelLayout->addAction(createAction("Medium", ":/icon/icon/layout.svg", "@Medium7"),
                            QToolButton::InstantPopup,
                            SARibbonPannelItem::Medium);
    pannelLayout->addAction(createAction("Small", ":/icon/icon/layout.svg", "@Small8"), QToolButton::InstantPopup, SARibbonPannelItem::Small);
}

void MainWindow::createQuickAccessBar(SARibbonQuickAccessBar* quickAccessBar)
{
    quickAccessBar->addAction(createAction("save", ":/icon/icon/save.svg", "save-quickbar"));
    quickAccessBar->addSeparator();
    quickAccessBar->addAction(createAction("undo", ":/icon/icon/undo.svg"));
    quickAccessBar->addAction(createAction("redo", ":/icon/icon/redo.svg"));
    quickAccessBar->addSeparator();
    QMenu* m = new QMenu("Presentation File", this);
    m->setIcon(QIcon(":/icon/icon/presentationFile.svg"));
    for (int i = 0; i < 10; ++i) {
        m->addAction(createAction(QString("file%1").arg(i + 1), ":/icon/icon/file.svg"));
    }
    quickAccessBar->addMenu(m);

    QAction* customize = createAction("customize", ":/icon/icon/customize0.svg", "customize2");
    quickAccessBar->addAction(customize);
    connect(customize, &QAction::triggered, this, &MainWindow::onActionCustomizeTriggered);

    QAction* actionCustomizeAndSave = createAction("customize and save", ":/icon/icon/customize.svg");
    quickAccessBar->addAction(actionCustomizeAndSave);
    connect(actionCustomizeAndSave, &QAction::triggered, this, &MainWindow::onActionCustomizeAndSaveTriggered);
}

void MainWindow::createRightButtonGroup(SARibbonButtonGroupWidget* rightBar)
{
    QAction* actionHelp = createAction(tr("help"), ":/icon/icon/help.svg");
    connect(actionHelp, &QAction::triggered, this, &MainWindow::onActionHelpTriggered);
    rightBar->addAction(actionHelp);
}

void MainWindow::addSomeOtherAction()
{
    //添加其他的action，这些action并不在ribbon管理范围，主要用于SARibbonCustomizeWidget自定义用
    QAction* acttext1 = new QAction(("text action1"), this);
    QAction* acttext2 = new QAction(("text action2"), this);
    QAction* acttext3 = new QAction(("text action3"), this);
    QAction* acttext4 = new QAction(("text action4"), this);
    QAction* acttext5 = new QAction(("text action5"), this);

    QAction* actIcon1 = new QAction(QIcon(":/icon/icon/layout.svg"), ("action with icon"), this);

    m_actionTagText     = SARibbonActionsManager::UserDefineActionTag + 1;
    m_actionTagWithIcon = SARibbonActionsManager::UserDefineActionTag + 2;

    m_actMgr = new SARibbonActionsManager(this);  //申明过程已经自动注册所有action

    //以下注册特别的action
    m_actMgr->registeAction(acttext1, SARibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(acttext3, SARibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(acttext5, SARibbonActionsManager::CommonlyUsedActionTag);
    m_actMgr->registeAction(actIcon1, SARibbonActionsManager::CommonlyUsedActionTag);

    m_actMgr->registeAction(acttext1, m_actionTagText);
    m_actMgr->registeAction(acttext2, m_actionTagText);
    m_actMgr->registeAction(acttext3, m_actionTagText);
    m_actMgr->registeAction(acttext4, m_actionTagText);
    m_actMgr->registeAction(acttext5, m_actionTagText);

    m_actMgr->registeAction(actIcon1, m_actionTagWithIcon);

    m_actMgr->setTagName(SARibbonActionsManager::CommonlyUsedActionTag, tr("in common use"));  //
    m_actMgr->setTagName(m_actionTagText, tr("no icon action"));
    m_actMgr->setTagName(m_actionTagWithIcon, tr("have icon action"));
}

QAction* MainWindow::createAction(const QString& text, const QString& iconurl, const QString& objName)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(objName);
    return act;
}

QAction* MainWindow::createAction(const QString& text, const QString& iconurl)
{
    QAction* act = new QAction(this);
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(text);
    return act;
}

void MainWindow::onMenuButtonPopupCheckableTest(bool b)
{
    m_edit->append(QString("MenuButtonPopupCheckableTest : %1").arg(b));
}

void MainWindow::onInstantPopupCheckableTest(bool b)
{
    m_edit->append(QString("InstantPopupCheckableTest : %1").arg(b));
}

void MainWindow::onDelayedPopupCheckableTest(bool b)
{
    m_edit->append(QString("DelayedPopupCheckableTest : %1").arg(b));
}

void MainWindow::onMenuButtonPopupCheckabletriggered(bool b)
{
    m_edit->append(QString("MenuButtonPopupCheckabletriggered : %1").arg(b));
}

void MainWindow::onInstantPopupCheckabletriggered(bool b)
{
    m_edit->append(QString("InstantPopupCheckabletriggered : %1").arg(b));
}

void MainWindow::onDelayedPopupCheckabletriggered(bool b)
{
    Q_UNUSED(b);
    m_edit->append(tr("The SARibbonToolButton::setPopupMode(QToolButton::DelayedPopup) method "
                      "can be used to set the menu pop-up method to delayed pop-up. "
                      "This also demonstrates manually setting text wrapping"));
    // cn:使用SARibbonToolButton::setPopupMode(QToolButton::DelayedPopup)方法可以设置菜单弹出方式为延迟弹出，这里也演示了手动设置文本的换行
}
