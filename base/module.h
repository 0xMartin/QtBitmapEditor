#ifndef MODULE_H
#define MODULE_H

/**
 * @brief The Module class
 */
class Module {
public:

    virtual ~Module() {
    }

    /**
     * @brief Modulu priradi pointer na kontext aplikace
     * @param context - Kontext aplikace
     */
    void setContext(void *context) {
        this->context = context;
    }

    /**
     * @brief Navrati ukazatel na kontext aplikace
     * @return Kontext aplikace
     */
    void *getContext() const {
        return this->context;
    }

protected:
    void *context; /** Ukazatel na kontext aplikace*/
};

#endif // MODULE_H
