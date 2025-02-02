//
// Created by rambod on 2/3/2025.
// Updated by rambod on [date]
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomDocument>
#include <QMap>

class QListWidget;
class QTabWidget;
class QTextBrowser;
class QTextEdit;
class QTableWidget;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    private slots:
        void openFile();
    void onTagSelected();
    void showFullSummary();
    void saveXML();
    void reloadXML();
    void filterDetails(const QString &filterText);

protected:
    // Ensure that the Tag Details tab is created after the window shows.
    void showEvent(QShowEvent *event) override;

private:
    // UI pointers
    QListWidget     *listWidget;
    QTabWidget      *detailsTabWidget;
    QTextBrowser    *summaryBrowser;
    QTextEdit       *xmlEditor;
    QTableWidget    *detailsTable;
    QLineEdit       *filterLineEdit;
    QPushButton     *showSummaryButton;

    // Data members
    QMap<QString, int> tagCountMap;
    QString currentFileName;
    QDomDocument xmlDoc;
    QString fullSummaryHtml;

    // Utility functions
    void countTags(const QDomNode &node, QMap<QString, int> &tagCounts);
    void countAttributes(const QDomNode &node, int &attrCount);
};

#endif // MAINWINDOW_H
