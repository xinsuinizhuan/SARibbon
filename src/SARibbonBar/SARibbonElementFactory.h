﻿#ifndef SARIBBONELEMENTFACTORY_H
#define SARIBBONELEMENTFACTORY_H
#include "SARibbonGlobal.h"
#include <QColor>
#include <QMargins>
#include <QSize>
#include <QScopedPointer>
#include "SARibbonBar.h"
#include "SARibbonPannel.h"
#include "SARibbonStyleOption.h"
class QWidget;
class SARibbonBar;
class SARibbonTabBar;
class SARibbonApplicationButton;
class SARibbonCategory;
class SARibbonContextCategory;
class SARibbonPannel;
class SARibbonSeparatorWidget;
class SARibbonGallery;
class SARibbonGalleryGroup;
class SARibbonToolButton;
class SARibbonControlButton;
class SARibbonButtonGroupWidget;
class SARibbonStackedWidget;
class SARibbonQuickAccessBar;
class SARibbonPannelOptionButton;

///
/// \brief SARibbon的子元素创建的工厂，SARibbon内部创建子元素都通过SARibbonElementFactory来创建
///
/// 由于SARibbonBar是一个复合控件，很多子窗口组合而成，有些部件的创建如果想继承，那么就需要这个工厂类来处理
/// 如SARibbonCategory，可以重载此类的createRibbonCategory,返回重载的类的实例
///
class SA_RIBBON_EXPORT SARibbonElementFactory
{
public:
    SARibbonElementFactory();
    virtual ~SARibbonElementFactory();
    virtual SARibbonTabBar* createRibbonTabBar(QWidget* parent);
    virtual SARibbonApplicationButton* createRibbonApplicationButton(QWidget* parent);
    virtual SARibbonCategory* createRibbonCategory(QWidget* parent);
    virtual SARibbonContextCategory* createRibbonContextCategory(QWidget* parent);
    virtual SARibbonPannel* createRibbonPannel(QWidget* parent);
    virtual SARibbonSeparatorWidget* createRibbonSeparatorWidget(int value, QWidget* parent);
    virtual SARibbonSeparatorWidget* createRibbonSeparatorWidget(QWidget* parent);
    virtual SARibbonGallery* createRibbonGallery(QWidget* parent);
    virtual SARibbonGalleryGroup* createRibbonGalleryGroup(QWidget* parent);
    virtual SARibbonToolButton* createRibbonToolButton(QWidget* parent);
    virtual SARibbonStackedWidget* createRibbonStackedWidget(SARibbonBar* parent);

    //创建隐藏ribbon的按钮代理函数
    virtual SARibbonControlButton* createHidePannelButton(SARibbonBar* parent);
    virtual SARibbonButtonGroupWidget* craeteButtonGroupWidget(QWidget* parent);
    virtual SARibbonQuickAccessBar* createQuickAccessBar(QWidget* parent);

    // SARibbonStyleOption可以进行继承，此函数无需设置为虚函数
    SARibbonStyleOption& getRibbonStyleOption();
    void setRibbonStyleOption(SARibbonStyleOption* opt);

    //创建SARibbonPannelOptionButton
    virtual SARibbonPannelOptionButton* createRibbonPannelOptionButton(SARibbonPannel* pannel);

private:
    QScopedPointer< SARibbonStyleOption > mStyleOption;
};

#endif  // SARIBBONELEMENTCREATEDELEGATE_H
