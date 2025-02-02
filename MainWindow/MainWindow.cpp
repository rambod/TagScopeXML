//
// Created by rambod on 2/3/2025.
// Updated by rambod on [date]
//

#include "MainWindow.h"

#include <QApplication>
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QListWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QFileDialog>
#include <QDomDocument>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QFont>
#include <QStatusBar>
#include <QPair>
#include <algorithm>
#include <QToolBar>
#include <QAction>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QShowEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("XML Tag Analyzer & Editor");

    // Create central widget and main layout.
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    // Top control: Open File and Show Full Summary buttons.
    QHBoxLayout *topLayout = new QHBoxLayout();
    QPushButton *openButton = new QPushButton("Open XML File", this);
    openButton->setFixedHeight(40);
    openButton->setStyleSheet(
        "QPushButton { background-color: #0078d7; color: white; border-radius: 5px; font-size: 16px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #005a9e; }"
    );
    topLayout->addWidget(openButton);

    showSummaryButton = new QPushButton("Show Full Summary", this);
    showSummaryButton->setFixedHeight(40);
    showSummaryButton->setStyleSheet(
        "QPushButton { background-color: #0078d7; color: white; border-radius: 5px; font-size: 16px; padding: 5px 15px; }"
        "QPushButton:hover { background-color: #005a9e; }"
    );
    showSummaryButton->setEnabled(false);
    topLayout->addWidget(showSummaryButton);

    mainLayout->addLayout(topLayout);

    // Splitter: Left panel (tag list) and Right panel (tabbed details).
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // Left panel: Tag list.
    listWidget = new QListWidget(this);
    listWidget->setMinimumWidth(220);
    listWidget->setStyleSheet(
        "QListWidget { background-color: #3c3c3c; font-size: 14px; border: 1px solid #555555; border-radius: 5px; color: #e0e0e0; }"
        "QListWidget::item { padding: 8px; }"
        "QListWidget::item:selected { background-color: #0078d7; color: white; }"
    );
    splitter->addWidget(listWidget);

    // Right panel: Tab widget.
    detailsTabWidget = new QTabWidget(this);
    detailsTabWidget->setStyleSheet(
        "QTabWidget::pane { border: 2px solid #0078d7; border-radius: 10px; padding: 5px; background-color: #2d2d30; }"
        "QTabBar::tab { background: #3c3c3c; color: #e0e0e0; padding: 8px; border-top-left-radius: 4px; border-top-right-radius: 4px; min-width: 80px; }"
        "QTabBar::tab:selected { background: #0078d7; color: white; }"
    );

    // Summary Tab.
    summaryBrowser = new QTextBrowser(this);
    summaryBrowser->setStyleSheet(
        "QTextBrowser { background-color: #2d2d30; font-size: 16px; color: #e0e0e0; padding: 15px; }"
    );
    detailsTabWidget->addTab(summaryBrowser, "Summary");

    // Tag Details Tab.
    QWidget *tagDetailsTab = new QWidget(this);
    QVBoxLayout *tagDetailsLayout = new QVBoxLayout(tagDetailsTab);
    tagDetailsLayout->setContentsMargins(5,5,5,5);
    tagDetailsLayout->setSpacing(5);
    filterLineEdit = new QLineEdit(this);
    filterLineEdit->setPlaceholderText("Filter details...");
    filterLineEdit->setStyleSheet(
        "QLineEdit { padding: 5px; font-size: 14px; border: 1px solid #555555; border-radius: 5px; background-color: #3c3c3c; color: #e0e0e0; }"
    );
    tagDetailsLayout->addWidget(filterLineEdit);
    detailsTable = new QTableWidget(this);
    detailsTable->setColumnCount(2);
    detailsTable->setHorizontalHeaderLabels(QStringList() << "Occurrence" << "Value");
    detailsTable->horizontalHeader()->setStretchLastSection(true);
    detailsTable->verticalHeader()->setVisible(false);
    detailsTable->setStyleSheet(
        "QTableWidget { background-color: #3c3c3c; font-size: 14px; color: #e0e0e0; }"
        "QHeaderView::section { background-color: #0078d7; color: white; padding: 4px; }"
    );
    tagDetailsLayout->addWidget(detailsTable, 1);
    detailsTabWidget->addTab(tagDetailsTab, "Tag Details");

    // Editor Tab.
    QWidget *editorTab = new QWidget(this);
    QVBoxLayout *editorLayout = new QVBoxLayout(editorTab);
    editorLayout->setContentsMargins(5,5,5,5);
    editorLayout->setSpacing(5);
    QToolBar *editorToolBar = new QToolBar(editorTab);
    QAction *saveAction = new QAction("Save XML", this);
    QAction *reloadAction = new QAction("Reload XML", this);
    editorToolBar->addAction(saveAction);
    editorToolBar->addAction(reloadAction);
    editorLayout->addWidget(editorToolBar);
    xmlEditor = new QTextEdit(this);
    xmlEditor->setStyleSheet(
        "QTextEdit { background-color: #3c3c3c; color: #e0e0e0; font-size: 16px; padding: 10px; }"
    );
    editorLayout->addWidget(xmlEditor, 1);
    detailsTabWidget->addTab(editorTab, "Editor");

    splitter->addWidget(detailsTabWidget);
    splitter->setStretchFactor(1, 1);
    mainLayout->addWidget(splitter);

    setCentralWidget(centralWidget);
    statusBar();

    // Connections.
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onTagSelected);
    connect(showSummaryButton, &QPushButton::clicked, this, &MainWindow::showFullSummary);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveXML);
    connect(reloadAction, &QAction::triggered, this, &MainWindow::reloadXML);
    connect(filterLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterDetails);
}

void MainWindow::openFile() {
    currentFileName = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml)");
    if (currentFileName.isEmpty())
        return;

    QFile file(currentFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::warning(this, "Error", "Failed to parse XML file.");
        return;
    }
    file.close();

    xmlDoc = doc; // Store for later use

    // Load XML text into editor.
    xmlEditor->setPlainText(doc.toString(4));

    // Compute tag counts.
    QMap<QString, int> tagCounts;
    QDomElement root = doc.documentElement();
    countTags(root, tagCounts);
    tagCountMap = tagCounts;

    // Compute attribute count.
    int totalAttributes = 0;
    countAttributes(root, totalAttributes);

    // Update tag list.
    listWidget->clear();
    for (auto it = tagCounts.begin(); it != tagCounts.end(); ++it) {
        listWidget->addItem(it.key());
    }

    // Overall statistics.
    int totalTagsCount = 0;
    for (int count : tagCounts)
        totalTagsCount += count;
    int uniqueTags = tagCounts.size();

    // Most frequent tag.
    QString mostFrequentTag;
    int maxCount = 0;
    for (auto it = tagCounts.begin(); it != tagCounts.end(); ++it) {
        if (it.value() > maxCount) {
            maxCount = it.value();
            mostFrequentTag = it.key();
        }
    }

    // Build sorted frequency table.
    QList<QPair<QString, int>> tagList;
    for (auto it = tagCounts.begin(); it != tagCounts.end(); ++it)
        tagList.append(qMakePair(it.key(), it.value()));
    std::sort(tagList.begin(), tagList.end(), [](const QPair<QString,int> &a, const QPair<QString,int> &b) {
        return a.second > b.second;
    });

    QString tableHtml = "<table style='width:100%; border-collapse:collapse;'>"
                        "<tr style='background-color:#0078d7; color:white;'><th style='padding:5px; text-align:left;'>Tag</th><th style='padding:5px; text-align:right;'>Count</th></tr>";
    for (const auto &pair : tagList) {
        tableHtml += QString("<tr style='border-bottom:1px solid #555555;'><td style='padding:5px;'>%1</td><td style='padding:5px; text-align:right;'>%2</td></tr>")
                         .arg(pair.first).arg(pair.second);
    }
    tableHtml += "</table>";

    fullSummaryHtml = QString(
        "<h2>XML Summary</h2>"
        "<p><b>File:</b> %1</p>"
        "<p><b>Total Elements:</b> %2</p>"
        "<p><b>Unique Tags:</b> %3</p>"
        "<p><b>Total Attributes:</b> %4</p>"
        "<p><b>Most Frequent Tag:</b> %5 (%6 times)</p>"
        "<hr>"
        "<h3>Tag Frequency Table</h3>%7"
        "<p style='font-style:italic;'>Select a tag from the list for details.</p>"
    ).arg(currentFileName).arg(totalTagsCount).arg(uniqueTags)
     .arg(totalAttributes).arg(mostFrequentTag).arg(maxCount).arg(tableHtml);

    summaryBrowser->setHtml(fullSummaryHtml);
    detailsTabWidget->setCurrentIndex(0); // Show Summary tab.
    statusBar()->showMessage(QString("Loaded file: %1    Total Elements: %2    Unique Tags: %3")
                               .arg(currentFileName).arg(totalTagsCount).arg(uniqueTags));

    showSummaryButton->setEnabled(true);
}

void MainWindow::onTagSelected() {
    QListWidgetItem *item = listWidget->currentItem();
    if (!item)
        return;
    QString tagName = item->text();

    // Find all elements with the selected tag.
    QList<QDomElement> elements;
    QDomNodeList nodeList = xmlDoc.elementsByTagName(tagName);
    for (int i = 0; i < nodeList.count(); ++i) {
        QDomNode n = nodeList.at(i);
        if (n.isElement())
            elements.append(n.toElement());
    }

    // Configure the details table.
    detailsTable->clearContents();
    detailsTable->setRowCount(elements.count());
    detailsTable->setColumnCount(2);
    QStringList headers;
    headers << "Occurrence" << "Value";
    detailsTable->setHorizontalHeaderLabels(headers);
    detailsTable->horizontalHeader()->setStretchLastSection(true);
    detailsTable->verticalHeader()->setVisible(false);

    // Fill the table with each element's value.
    for (int i = 0; i < elements.count(); ++i) {
        QDomElement elem = elements.at(i);
        QString value;
        QDomNode child = elem.firstChild();
        if (child.isNull() || !child.toText().isNull()) {
            // Use text content if available.
            value = elem.text().trimmed();
        } else {
            // Otherwise list immediate child tags and texts.
            while (!child.isNull()) {
                if (child.isElement()) {
                    QDomElement childElem = child.toElement();
                    value += QString("<b>%1</b>: %2; ")
                                 .arg(childElem.tagName(), childElem.text().trimmed());
                }
                child = child.nextSibling();
            }
        }
        detailsTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        detailsTable->setItem(i, 1, new QTableWidgetItem(value));
    }
    // Clear any existing filter.
    filterLineEdit->clear();
    detailsTabWidget->setCurrentIndex(1); // Switch to Tag Details tab.
}

void MainWindow::showFullSummary() {
    summaryBrowser->setHtml(fullSummaryHtml);
    detailsTabWidget->setCurrentIndex(0);
}

void MainWindow::saveXML() {
    if (currentFileName.isEmpty()) {
        QMessageBox::information(this, "Info", "No file loaded.");
        return;
    }
    QFile file(currentFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Cannot write file: " + file.errorString());
        return;
    }
    QString xmlContent = xmlEditor->toPlainText();
    file.write(xmlContent.toUtf8());
    file.close();
    QMessageBox::information(this, "Saved", "XML file saved successfully.");
    openFile(); // Refresh stats.
}

void MainWindow::reloadXML() {
    if (currentFileName.isEmpty()) {
        QMessageBox::information(this, "Info", "No file loaded.");
        return;
    }
    QFile file(currentFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        QMessageBox::warning(this, "Error", "Failed to parse XML file.");
        return;
    }
    file.close();
    xmlEditor->setPlainText(doc.toString(4));
}

void MainWindow::filterDetails(const QString &filterText) {
    for (int row = 0; row < detailsTable->rowCount(); ++row) {
        QTableWidgetItem *item = detailsTable->item(row, 1); // Value column.
        bool match = item->text().contains(filterText, Qt::CaseInsensitive);
        detailsTable->setRowHidden(row, !match);
    }
}

void MainWindow::countTags(const QDomNode &node, QMap<QString, int> &tagCounts) {
    QDomNode child = node.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            QString tagName = child.nodeName();
            tagCounts[tagName] += 1;
            countTags(child, tagCounts);
        }
        child = child.nextSibling();
    }
}

void MainWindow::countAttributes(const QDomNode &node, int &attrCount) {
    QDomNode child = node.firstChild();
    while (!child.isNull()) {
        if (child.isElement()) {
            QDomElement element = child.toElement();
            attrCount += element.attributes().count();
            countAttributes(child, attrCount);
        }
        child = child.nextSibling();
    }
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    // If the detailsTabWidget has only 2 tabs (Summary and Editor), add the Tag Details tab.
    if (detailsTabWidget->count() < 3) {
        QWidget *tagDetailsTab = new QWidget(this);
        QVBoxLayout *tagDetailsLayout = new QVBoxLayout(tagDetailsTab);
        tagDetailsLayout->setContentsMargins(5, 5, 5, 5);
        tagDetailsLayout->setSpacing(5);
        filterLineEdit = new QLineEdit(this);
        filterLineEdit->setPlaceholderText("Filter details...");
        filterLineEdit->setStyleSheet("QLineEdit { padding: 5px; font-size: 14px; border: 1px solid #555555; border-radius: 5px; background-color: #3c3c3c; color: #e0e0e0; }");
        tagDetailsLayout->addWidget(filterLineEdit);
        detailsTable = new QTableWidget(this);
        detailsTable->setColumnCount(2);
        detailsTable->setHorizontalHeaderLabels(QStringList() << "Occurrence" << "Value");
        detailsTable->horizontalHeader()->setStretchLastSection(true);
        detailsTable->verticalHeader()->setVisible(false);
        detailsTable->setStyleSheet("QTableWidget { background-color: #3c3c3c; font-size: 14px; color: #e0e0e0; }"
                                    "QHeaderView::section { background-color: #0078d7; color: white; padding: 4px; }");
        tagDetailsLayout->addWidget(detailsTable, 1);
        detailsTabWidget->addTab(tagDetailsTab, "Tag Details");

        connect(filterLineEdit, &QLineEdit::textChanged, this, &MainWindow::filterDetails);
    }
}
