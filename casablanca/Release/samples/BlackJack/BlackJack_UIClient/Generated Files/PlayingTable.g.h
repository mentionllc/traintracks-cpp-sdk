﻿

#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------

namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class Grid;
                ref class TextBlock;
                ref class AppBar;
                ref class Button;
                ref class TextBox;
                ref class StackPanel;
            }
        }
    }
}

namespace BlackjackClient
{
    partial ref class PlayingTable : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::Grid^ LayoutRoot;
        private: ::Windows::UI::Xaml::Controls::Grid^ dealerGrid;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ resultLabel;
        private: ::Windows::UI::Xaml::Controls::AppBar^ TopBar;
        private: ::Windows::UI::Xaml::Controls::AppBar^ BottomBar;
        private: ::Windows::UI::Xaml::Controls::Button^ exitButton;
        private: ::Windows::UI::Xaml::Controls::Button^ joinButton;
        private: ::Windows::UI::Xaml::Controls::Button^ leaveButton;
        private: ::Windows::UI::Xaml::Controls::TextBox^ serverAddress;
        private: ::Windows::UI::Xaml::Controls::TextBox^ playerName;
        private: ::Windows::UI::Xaml::Controls::Grid^ playerGrid;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ playerPanel;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ hitButton;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ stayButton;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ doubleButton;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ button1;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ label1;
        private: ::Windows::UI::Xaml::Controls::TextBox^ betBox;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ buyInsuranceButton;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ buyInsuranceLabel;
        private: ::Windows::UI::Xaml::Controls::TextBox^ buyInsuranceTextBox;
    };
}

