#!/bin/bash

echo "🔄 Converting Smart Farm Project Proposal to PDF..."
echo "=================================================="

# Check if pandoc is installed
if ! command -v pandoc &> /dev/null; then
    echo "❌ Pandoc not found. Installing..."
    sudo apt update
    sudo apt install -y pandoc texlive-latex-base texlive-latex-extra texlive-fonts-recommended
fi

# Create PDF with enhanced formatting
echo "📄 Generating PDF with professional formatting..."

pandoc SMART_FARM_PROJECT_PROPOSAL.md \
    -o "Smart_Farm_Cattle_Monitoring_System_Proposal.pdf" \
    --pdf-engine=pdflatex \
    --toc \
    --toc-depth=3 \
    --number-sections \
    --highlight-style=github \
    --variable geometry:margin=1in \
    --variable fontsize=11pt \
    --variable documentclass=article \
    --variable colorlinks=true \
    --variable linkcolor=blue \
    --variable urlcolor=blue \
    --variable toccolor=black \
    --include-in-header=<(cat << 'EOF'
\usepackage{fancyhdr}
\pagestyle{fancy}
\fancyhead[L]{Smart Farm Cattle Monitoring System}
\fancyhead[R]{\today}
\fancyfoot[C]{\thepage}
\usepackage{graphicx}
\usepackage{xcolor}
\definecolor{titlecolor}{RGB}{0,102,204}
EOF
) \
    --metadata title="Smart Farm Cattle Monitoring System" \
    --metadata subtitle="Comprehensive Project Proposal" \
    --metadata author="Smart Agriculture Solutions Team" \
    --metadata date="November 7, 2025"

if [ $? -eq 0 ]; then
    echo "✅ PDF generated successfully!"
    echo "📁 File: Smart_Farm_Cattle_Monitoring_System_Proposal.pdf"
    echo ""
    echo "📊 Document Statistics:"
    echo "======================"
    wc -w SMART_FARM_PROJECT_PROPOSAL.md | awk '{print "Words: " $1}'
    wc -l SMART_FARM_PROJECT_PROPOSAL.md | awk '{print "Lines: " $1}'
    ls -lh Smart_Farm_Cattle_Monitoring_System_Proposal.pdf | awk '{print "PDF Size: " $5}'
    echo ""
    echo "📋 Document Sections Include:"
    echo "=============================="
    grep -n "^## " SMART_FARM_PROJECT_PROPOSAL.md | head -20
    echo ""
    echo "💰 Budget Summary:"
    echo "=================="
    echo "Total Hardware Cost: $3,244 USD / 3,244,000 RWF"
    echo "Development Cost: $3,100 USD / 3,100,000 RWF"
    echo "Total Project Cost: $6,344 USD / 6,344,000 RWF"
    echo "Expected ROI: 97% over 5 years"
    echo ""
    echo "🎯 Key Deliverables:"
    echo "==================="
    echo "✅ 25 Cow monitoring collars"
    echo "✅ 2 Milk station nodes"
    echo "✅ 1 Central gateway dashboard"
    echo "✅ Cloud platform and mobile app"
    echo "✅ Training and support"
else
    echo "❌ PDF generation failed!"
    echo "Trying alternative method with simpler formatting..."
    
    # Fallback method
    pandoc SMART_FARM_PROJECT_PROPOSAL.md -o Smart_Farm_Proposal_Simple.pdf
    
    if [ $? -eq 0 ]; then
        echo "✅ Simple PDF generated: Smart_Farm_Proposal_Simple.pdf"
    else
        echo "❌ PDF generation failed. Please install pandoc and texlive packages."
    fi
fi

echo ""
echo "📧 Ready for distribution to:"
echo "============================"
echo "• Investors and venture capitalists"
echo "• Agricultural cooperatives"
echo "• Government agriculture departments"
echo "• International development organizations"
echo "• Technology partners"
echo ""