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

    /**
     * @brief Vybere font
     * @param font - Font
     */
    void setSelectedFont(const QFont &font);

public slots:
    /**
     * @brief Zobrazi dialog pro vyber fontu
     */
    void selectFont();

signals:
    /**
     * @brief Signal bude vyvolan v okamziku zmeni fontu
     */
    void fontChanged(const QFont &font);

protected:
    // aktualne vybrany font
    QFont font;

    /**
     * @brief Obnovy jmeno vybraneho fontu
     */
    void updateFontName();

};

#endif // FONTSELECTOR_H
