#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QPushButton>
#include <QColor>

class ColorPicker : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorPicker( QWidget* parent );

    void setColor( const QColor& color );
    const QColor& getColor() const;

    virtual void changeEvent(QEvent *);

public slots:
    void updateColor();
    void changeColor();

private:
    QColor color;
};

#endif // COLORPICKER_H
