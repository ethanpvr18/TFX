#pragma once

#include <JuceHeader.h>
#include <iostream>

using namespace std;

class TableComponent: public juce::Component, public juce::TableListBoxModel {
public:
    TableComponent()
    {
        table.setModel(this);
        table.setColour (juce::ListBox::outlineColourId, juce::Colours::grey);
                
        loadData();

        if (columnList != nullptr)
        {
            
            int i = 0;
            
            forEachXmlChildElement (*columnList, columnXml)
            {
                table.getHeader().addColumn (columnXml->getStringAttribute ("name"),
                                             columnXml->getIntAttribute ("columnId"),
                                             columnXml->getIntAttribute ("width"),
                                             columnXml->getIntAttribute ("width"),
                                             columnXml->getIntAttribute ("width"),
                                             18,
                                             i);
                
                i++;
            }
        }
                                        
        addAndMakeVisible (table);
    }

    int getNumRows() override
    {
        return numRows;
    }

    void paintRowBackground (juce::Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
    {
        auto alternateColour = getLookAndFeel().findColour(juce::ListBox::backgroundColourId)
                                               .interpolatedWith(getLookAndFeel()
                                               .findColour(juce::ListBox::textColourId), 0.03f);
        for(int columnNum = 0; columnNum < 3; columnNum++){
            if (rowIsSelected){
                g.fillAll (juce::Colours::lightblue);
                paintCell(g, rowNumber, columnNum, 200, 30, true);
            }else if (rowNumber % 2){
                g.fillAll (alternateColour);
                paintCell(g, rowNumber, columnNum, 200, 30, false);
            }else{
                paintCell(g, rowNumber, columnNum, 200, 30, false);
            }
        }
    }

    void paintCell (juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override
    {
        g.setColour (rowIsSelected ? juce::Colours::darkblue : getLookAndFeel().findColour (juce::ListBox::textColourId));
        g.setFont (font);
        
        if (auto* rowElement = dataList->getChildElement (rowNumber))
        {
            auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));
            
            g.drawText (text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }

        g.setColour (getLookAndFeel().findColour (juce::ListBox::backgroundColourId));
        g.fillRect (width - 1, 0, 1, height);
    }
    
    void resized() override {
        table.setBounds(getLocalBounds());
    }
private:
    juce::TableListBox table  { {}, this };
    juce::Font font           { 14.0f };

    std::unique_ptr<juce::XmlElement> data;
    juce::XmlElement* columnList = nullptr;
    juce::XmlElement* dataList = nullptr;
    int numRows = 0;

    void loadData()
    {
        //Change to your resources path for now, will fix
        juce::File tableFile = juce::File("/Users/ethan/TFX/Resources/TableData.xml");

        if (tableFile.exists())
        {
            data = juce::XmlDocument::parse (tableFile);

            dataList   = data->getChildByName ("DATA");
            columnList = data->getChildByName ("HEADERS");     

            numRows = dataList->getNumChildElements();                 
        }
    }

    juce::String getAttributeNameForColumnId (const int columnId) const
    {
        forEachXmlChildElement (*columnList, columnXml)
        {
            if (columnXml->getIntAttribute ("columnId") == columnId)
                return columnXml->getStringAttribute ("name");
        }

        return {};
    }
};

class MainComponent : public juce::Component
{
public:
    MainComponent() : juce::Component()
    {
        addAndMakeVisible (table);
        
        setSize (1200, 600);
    }

    void resized() override
    {
        table.setBounds (0, 0, getWidth(), getHeight());
    }

private:
    TableComponent table;
};