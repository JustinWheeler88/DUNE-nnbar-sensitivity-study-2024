// Set canvas to DUNE plotting guidelines
// 6:4 axes ratio

#include "TStyle.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TColor.h"

void duneStyle_6to4_sample() {
    // Use Helvetica font
    Int_t duneFont = 42; // Helvetica
    Double_t duneWidth = 2;
    Double_t duneTSize = 0.055;

    // Create and set style
    TStyle *duneStyle = new TStyle("duneStyle", "DUNE plots style");

    duneStyle->SetFillColor(1);
    duneStyle->SetFillStyle(1001);
    duneStyle->SetFrameFillColor(0);
    duneStyle->SetFrameBorderMode(0);
    duneStyle->SetPadBorderMode(0);
    duneStyle->SetPadColor(0);
    duneStyle->SetCanvasBorderMode(0);
    duneStyle->SetCanvasColor(0);
    duneStyle->SetStatColor(0);
    duneStyle->SetLegendBorderSize(0);

    // Gradient color palette
    // const Int_t NRGBs = 7;
    // const Int_t NCont = 512;
    // Double_t stops[NRGBs] = { 0.00, 0.00001, 0.00002, 0.3, 0.6, 0.85, 1.00 };
    // Double_t red[NRGBs] = { 0.00, 1.00, 1.00, 0.42, 0.87, 1.00, 0.51 };
    // Double_t green[NRGBs] = { 0.05, 1.00, 1.00, 0.76, 1.00, 0.20, 0.00 };
    // Double_t blue[NRGBs] = { 0.87, 1.00, 1.00, 0.21, 0.12, 0.00, 0.00 };

    // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    // duneStyle->SetNumberContours(NCont);
    duneStyle->SetPalette(kCividis);

    // Paper and margin sizes
    duneStyle->SetPaperSize(20, 26);
    duneStyle->SetPadTopMargin(0.10);
    duneStyle->SetPadRightMargin(0.20); // Increase for colz plots
    duneStyle->SetPadBottomMargin(0.22);
    duneStyle->SetPadLeftMargin(0.18);

    // Canvas to produce ~square plot
    duneStyle->SetCanvasDefH(800);
	duneStyle->SetCanvasDefW(10000);

    // Use large fonts
    duneStyle->SetTextFont(duneFont);
    duneStyle->SetTextSize(duneTSize);
    duneStyle->SetLabelFont(duneFont, "x");
    duneStyle->SetLabelFont(duneFont, "y");
    duneStyle->SetLabelFont(duneFont, "z");
    duneStyle->SetLabelSize(1.2 * duneTSize, "x");
    duneStyle->SetLabelSize(1.2 * duneTSize, "y");
    duneStyle->SetLabelSize(1.2 * duneTSize, "z");
    duneStyle->SetTitleFont(duneFont);
    duneStyle->SetTitleFont(duneFont, "x");
    duneStyle->SetTitleFont(duneFont, "y");
    duneStyle->SetTitleFont(duneFont, "z");
    duneStyle->SetTitleSize(1.2 * duneTSize, "x");
    duneStyle->SetTitleSize(1.2 * duneTSize, "y");
    duneStyle->SetTitleSize(1.2 * duneTSize, "z");

    // Line width and markers
    duneStyle->SetLineWidth(duneWidth);
    duneStyle->SetFrameLineWidth(duneWidth);
    duneStyle->SetHistLineWidth(duneWidth);
    duneStyle->SetFuncWidth(duneWidth);
    duneStyle->SetGridWidth(duneWidth);
    duneStyle->SetMarkerStyle(20);
    duneStyle->SetMarkerSize(0.8);

    // Label offsets
    duneStyle->SetLabelOffset(0.010, "X");
    duneStyle->SetLabelOffset(0.010, "Y");

    // Do not display histogram decorations by default
    duneStyle->SetOptStat(0);
    duneStyle->SetOptTitle(0);
    duneStyle->SetOptFit(1);

    // Titles
    //duneStyle->SetTitleOffset(1.2, "X");
    duneStyle->SetTitleOffset(1.0, "Y");
    duneStyle->SetTitleOffset(1.2, "Z");
    duneStyle->SetTitleFillColor(0);
    duneStyle->SetTitleStyle(0);
    duneStyle->SetTitleBorderSize(0);
    duneStyle->SetTitleFont(duneFont, "title");
    duneStyle->SetTitleX(0.0);
    duneStyle->SetTitleY(1.0);
    duneStyle->SetTitleW(1.0);
    duneStyle->SetTitleH(0.05);

    // Statistics box
    duneStyle->SetStatBorderSize(0);
    duneStyle->SetStatFont(duneFont);
    duneStyle->SetStatFontSize(0.05);
    duneStyle->SetStatX(0.9);
    duneStyle->SetStatY(0.9);
    duneStyle->SetStatW(0.25);
    duneStyle->SetStatH(0.15);

    // Ticks
    duneStyle->SetPadTickX(1);
    duneStyle->SetPadTickY(1);

    // Histogram divisions
    duneStyle->SetNdivisions(505, "x");
    duneStyle->SetNdivisions(510, "y");

    gROOT->SetStyle("duneStyle");
    gROOT->ForceStyle();

    // // Add DUNE watermark
    // TPaveText* duneWatermark = new TPaveText(duneStyle->GetPadLeftMargin() + 0.05,
    //                                          0.87 - duneStyle->GetPadTopMargin(),
    //                                          duneStyle->GetPadLeftMargin() + 0.20,
    //                                          0.95 - duneStyle->GetPadTopMargin(),
    //                                          "BRNDC");
    // duneWatermark->SetFillColor(0);
    // duneWatermark->SetTextAlign(12);
    // duneWatermark->SetBorderSize(0);
    
    // TText *duneLabel = new TText();
    // duneLabel->SetTextFont(duneFont);
    // duneLabel->SetTextColor(1);
    // duneLabel->SetTextSize(duneTSize);
    // duneLabel->SetTextAlign(12);
    // duneLabel->SetText("DUNE Preliminary");

    // duneWatermark->AddText(duneLabel);
    // duneWatermark->Draw();

    TLatex *duneLatex = new TLatex();
    duneLatex->SetTextFont(duneFont);
    duneLatex->SetTextColor(1);
    duneLatex->SetTextSize(duneTSize);
    duneLatex->SetTextAlign(12);

    // // Additional annotations
    // duneLatex->DrawLatexNDC(0.7, 0.8, "Run: X, Subrun: Y, Event: Z");
    // duneLatex->DrawLatexNDC(0.7, 0.75, "Date: YYYY-MM-DD");
}
