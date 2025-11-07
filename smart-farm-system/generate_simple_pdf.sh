#!/bin/bash

# Simple PDF Generator for Smart Farm Project
echo "==================================================================================="
echo "                      Smart Farm System PDF Generator v2.0"
echo "==================================================================================="
echo ""

# Change to project directory
cd /home/admino/smart-farm-system

# Check if pandoc is available
if ! command -v pandoc &> /dev/null; then
    echo "Error: Pandoc is required but not installed."
    echo "Please install with: sudo apt-get install pandoc"
    exit 1
fi

echo "Step 1: Generating Executive Summary PDF..."
pandoc "SMART_FARM_EXECUTIVE_SUMMARY.md" \
    -o "Smart_Farm_Executive_Summary.pdf" \
    --pdf-engine=pdflatex \
    --variable geometry:margin=1in \
    --variable fontsize=11pt \
    --variable documentclass=article \
    --toc \
    --highlight-style=tango

if [ $? -eq 0 ]; then
    echo "SUCCESS: Executive Summary PDF generated successfully!"
    echo "Location: Smart_Farm_Executive_Summary.pdf"
else
    echo "WARNING: Executive Summary PDF generation had issues"
fi

echo ""
echo "Step 2: Generating Technical Specifications PDF..."
if [ -f "TECHNICAL_SPECIFICATIONS.md" ]; then
    pandoc "TECHNICAL_SPECIFICATIONS.md" \
        -o "Smart_Farm_Technical_Specifications.pdf" \
        --pdf-engine=pdflatex \
        --variable geometry:margin=1in \
        --variable fontsize=11pt \
        --variable documentclass=article \
        --toc \
        --highlight-style=tango
    
    if [ $? -eq 0 ]; then
        echo "SUCCESS: Technical Specifications PDF generated!"
        echo "Location: Smart_Farm_Technical_Specifications.pdf"
    else
        echo "WARNING: Technical Specifications PDF generation had issues"
    fi
else
    echo "SKIPPED: TECHNICAL_SPECIFICATIONS.md not found"
fi

echo ""
echo "Step 3: Generating Budget Analysis PDF..."
if [ -f "DETAILED_BUDGET.md" ]; then
    pandoc "DETAILED_BUDGET.md" \
        -o "Smart_Farm_Budget_Analysis.pdf" \
        --pdf-engine=pdflatex \
        --variable geometry:margin=1in \
        --variable fontsize=11pt \
        --variable documentclass=article \
        --toc \
        --highlight-style=tango
    
    if [ $? -eq 0 ]; then
        echo "SUCCESS: Budget Analysis PDF generated!"
        echo "Location: Smart_Farm_Budget_Analysis.pdf"
    else
        echo "WARNING: Budget Analysis PDF generation had issues"
    fi
else
    echo "SKIPPED: DETAILED_BUDGET.md not found"
fi

echo ""
echo "==================================================================================="
echo "                              PDF Generation Complete"
echo "==================================================================================="
echo ""
echo "Generated Files:"
ls -la *.pdf 2>/dev/null | grep "Smart_Farm" || echo "No PDF files found"
echo ""
echo "Total size:"
du -sh *.pdf 2>/dev/null | grep "Smart_Farm" || echo "No PDF files to measure"
echo ""
echo "Files are ready for:"
echo "- Investor presentations"
echo "- Grant applications"
echo "- Technical reviews"
echo "- Stakeholder distribution"
echo ""
echo "==================================================================================="