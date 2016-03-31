#ifndef RAM_H
#define RAM_H
#define sint long int

class RAM
{
public:
    RAM();
    sint getValueAt(sint);
    void setValueAt(sint, sint);

private:
    sint* memory;
};

#endif // RAM_H
