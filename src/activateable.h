#ifndef ACTIVATEABLE_H
#define ACTIVATEABLE_H

/*
 * Keeps track of who wants a sensor running. Each part is counted rather
 * than kept as a single bit: a page that is being popped is only destroyed
 * after its transition, so the next page can already have activated the same
 * sensor by the time the old one deactivates it. With a bit mask that late
 * deactivate switched the sensor off underneath the page on screen.
 */
class Activateable
{
  private:
    static const unsigned NUM_PARTS = 3;

    unsigned m_partCount[NUM_PARTS] = {};

    static int partIndex(unsigned part);

  public:
    static const unsigned PART_PAGE = (1 << 0);
    static const unsigned PART_COVER = (1 << 1);
    static const unsigned PART_LOGGING = (1 << 2);

    Activateable() = default;
    virtual ~Activateable() = default;

    virtual void activate(unsigned requestingPart);
    virtual void deactivate(unsigned requestingPart);

    virtual bool isActive() const;
    virtual bool isPartActive(unsigned part) const;
};

#endif // ACTIVATEABLE_H
