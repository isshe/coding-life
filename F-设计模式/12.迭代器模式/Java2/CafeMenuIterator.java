package Java;

import java.util.*;

public class CafeMenuIterator implements Iterator {
    Hashtable<String, MenuItem> items;
    java.util.Iterator<MenuItem> it;
    int position = 0;

    public CafeMenuIterator(Hashtable<String, MenuItem> items) {
        this.items = items;
        it = items.values().iterator();
    }

    public MenuItem next() {
        return it.next();
    }

    public boolean hasNext() {
        return it.hasNext();
    }
}