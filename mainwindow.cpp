#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <qtextedit.h>
#include <QFontDialog>
#include <QFont>
#include <QFontDatabase>
#include <QTextCursor>
#include <QColor>
#include <QColorDialog>
#include <QSyntaxHighlighter>
#include <QtGui>

class mySyntaxHighLighter: public QSyntaxHighlighter //Syntax Highligher class
{

  public:
    mySyntaxHighLighter(QTextDocument* document):
    QSyntaxHighlighter(document)
    {
    };

    ~ mySyntaxHighLighter()
    {};

    void highlightBlock(const QString &text)
    {

    enum { NormalState = -1, CStyleComment };

        int state = previousBlockState();
        int start = 0;

      for (int i = 0; i < text.length(); ++i)
      {

          if (state == CStyleComment)
          {
          if (text.mid(i, 2) == "*/")
          {
              state = NormalState;
              setFormat(start, i - start + 2, Qt::blue);
          }
          }
          else
          {
          if (text.mid(i, 2) == "//")
          {
              setFormat(i, text.length() - i, Qt::red);
              break;
          }
          else if (text.mid(i, 2) == "/*")
		            {
              start = i;
              state = CStyleComment;
          }
          }
      }
    };
};

                                             


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    mySyntaxHighLighter* highlighter = new mySyntaxHighLighter(ui->textEdit->document());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    mFilename="";
    ui->textEdit->setPlainText("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this,"Open a file");
    if(!file.isEmpty())
    {
        QFile sfile(file);
        if(sfile.open(QFile::ReadOnly | QFile::Text))
        {
            mFilename=file;
            QTextStream in(&sfile);
            QString text = in.readAll();
            sfile.close();
            ui->textEdit->setPlainText(text);
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(mFilename=="")
    {
        on_actionSave_As_triggered();
    }
    else
    {
        QFile sfile(mFilename);
        if(sfile.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&sfile);
            out<<ui->textEdit->toPlainText();
            sfile.flush();
            sfile.close();
        }
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file=QFileDialog::getSaveFileName(this,"Save Your File");
    if(file!="")
    {
        mFilename=file;
        on_actionSave_triggered();
    }
}

void MainWindow::on_actionFind_triggered()
{
    QString findtext = QInputDialog::getText(this,"Enter The Text","Text:");
    ui->textEdit->moveCursor(QTextCursor::Start);
    ui->textEdit->find(findtext,QTextDocument::FindWholeWords);
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}
