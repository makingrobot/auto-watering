/**
 * ESP32-Arduino-Framework
 * Arduino开发环境下适用于ESP32芯片系列开发板的应用开发框架。
 * 
 * Author: Billy Zhang（billy_zh@126.com）
 */
#ifndef _ONEBUTTON_IMPL_H
#define _ONEBUTTON_IMPL_H

#include "button.h"
#include "board.h"

#include <Arduino.h>
#include <OneButton.h>

/**
 * 按键功能实现
 * 使用OneButton三方库
 */
class OneButtonImpl : public Button {
public:
    OneButtonImpl(const std::string& name, const int pin, const bool activeLow = true, const bool pullupActive = true) 
            : Button(name) {
        button_ = new OneButton(pin, activeLow, pullupActive);
    }

    void BindAction(const ButtonAction action) override {
        if (action == ButtonAction::Click) {
            OnClick([action, this](){
                OnEvent(action);
            });

        } else if (action == ButtonAction::DoubleClick) {
            OnDoubleClick([action, this](){
                OnEvent(action);
            });

        } else if (action == ButtonAction::LongPress) {
            OnLongPress([action, this](){
                OnEvent(action);
            });
            
        }
    }

    void OnClick(std::function<void()> click_func) override { 
        click_func_ = click_func;

        button_->attachClick([](void *param) {
            OneButtonImpl *_this = (OneButtonImpl*)param;
            _this->click_func_();
        }, this);
    }

    void OnClick(std::function<void(void*)> click_param_func, void *parameter) override  { 
        click_param_func_ = click_param_func;
        click_parameter_ = parameter;

        button_->attachClick([](void *param) {
            OneButtonImpl *_this = (OneButtonImpl*)param;
            _this->click_param_func_(_this->click_parameter_);
        }, this);
    }

    void OnDoubleClick(std::function<void()> doubleclick_func) override  { 
        doubleclick_func_ = doubleclick_func;

        button_->attachDoubleClick([](void *param) {
            OneButtonImpl *_this = (OneButtonImpl*)param;
            _this->doubleclick_func_();
        }, this);
    }

    void OnDoubleClick(std::function<void(void*)> doubleclick_param_func, void *parameter) override  { 
        doubleclick_param_func_ = doubleclick_param_func;
        doubleclick_parameter_ = parameter;

        button_->attachDoubleClick([](void *param) {
            OneButtonImpl *_this = (OneButtonImpl*)param;
            _this->doubleclick_param_func_(_this->doubleclick_parameter_);
        }, this);
    }

    void OnLongPress(std::function<void()> longpress_func) override {
        longpress_func_ = longpress_func;

        button_->attachLongPressStart([](void *param){
            OneButtonImpl *_this = (OneButtonImpl*)param;
            _this->longpress_start_ = millis();
        }, this);

        button_->attachLongPressStop([](void *param){
            OneButtonImpl *_this = (OneButtonImpl*)param;
            uint32_t duration = millis() - _this->longpress_start_;
            if (duration > _this->longpress_interval_) {
                _this->longpress_func_();
            }
        }, this);
    }

    void OnLongPress(std::function<void(void*)> longpress_param_func, void *parameter) override {
        longpress_param_func_ = longpress_param_func;
        longpress_parameter_ = parameter;

        button_->attachLongPressStart([](void *param){
            OneButtonImpl *_this = (OneButtonImpl*)param;
            _this->longpress_start_ = millis();
        }, this);

        button_->attachLongPressStop([](void *param){
            OneButtonImpl *_this = (OneButtonImpl*)param;
            uint32_t duration = millis() - _this->longpress_start_;
            if (duration > _this->longpress_interval_) {
                _this->longpress_param_func_(_this->longpress_parameter_);
            }
        }, this);
    }

    void SetLongPressIntervalMs(uint32_t interval) override {
        longpress_interval_ = interval;
        button_->setLongPressIntervalMs(interval);
    }

    bool isPressed() override {
        return button_->isLongPressed();
    }

    void Tick() override {
        button_->tick();
    }

private:
    void OnEvent(const ButtonAction action) {
        Board& board = Board::GetInstance();
        board.OnPhysicalButtonEvent(name(), action);
    }

    OneButton *button_;
    uint32_t longpress_interval_ = 5000;  // 5秒
    uint32_t longpress_start_ = 0; 

    std::function<void()> click_func_;
    std::function<void(void*)> click_param_func_;
    void *click_parameter_;

    std::function<void()> doubleclick_func_;
    std::function<void(void*)> doubleclick_param_func_;
    void *doubleclick_parameter_;

    std::function<void()> longpress_func_;
    std::function<void(void*)> longpress_param_func_;
    void *longpress_parameter_;

};

#endif //_ONEBUTTON_IMPL_H