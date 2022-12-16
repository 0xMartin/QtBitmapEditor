#include "fontselector.h"

#include <QFontDialog>


FontSelector::FontSelector(QWidget* parent) : QPushButton(parent)
{
    this->font = QFont("Helvetica [Cronyx]", 10);
    connect(this, SIGNAL(pressed()), this, SLOT(selectFont()));
    this->updateFontName();
    this->setToolTip(tr("Select Font"));
}

const QFont &FontSelector::getFont() const
{
    return this->font;
}

void FontSelector::selectFont()
{
    bool ok;
    QFont f = QFontDialog::getFont(&ok, this->font, this);
    if (ok) {
        this->font = f;
        emit this->fontChanged(this->font);
        this->updateFontName();
    }
}

void FontSelector::updateFontName()
{
    this->setText(this->font.family() + " " + QString::number(this->font.pointSize()));
}
