#ifndef FONTSELECTOR_H
#define FONTSELECTOR_H

#include <QPushButton>
#include <QFont>

/**
 * @brief The FontSelector class
 */
class FontSelector : public QPushButton
{
    Q_OBJECT
public:
    explicit FontSelector(QWidget* parent = nullptr);

    const QFont &getFont() const;

public slots:
    /**
     * @brief Zobrazi dialog pro vyber fontu
     */
    void selectFont();

signals:
    /**
     * @brief Signal bude vyvolan v okamziku zmeni fontu
     */
    void fontChanged(QFont &font);

protected:
    // aktualne vybrany font
    QFont font;

    /**
     * @brief Obnovy jmeno vybraneho fontu
     */
    void updateFontName();

};

#endif // FONTSELECTOR_H
