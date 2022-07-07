package Store.StoreData;

/**
 * Class that defines goods in the store.
 * <p>
 * In this version it basically encapsulates the name String as its attribute.
 * 
 * @author xvosci00 Martin Voščinár
 */
public class Goods {
    private String name;

    /**
     * Constructor that creates a Goods instance with the given name.
     * @param name name of Goods
     */
    public Goods(String name) {
        this.name = name;
    }

    /**
     * Get function for the Goods name.
     * @return Goods name String
     */
    public String getName() {
        return this.name;
    }

    /**
     * Overrides the default method to return the name attribute.
     * @return Goods name String
     */
    @Override
    public String toString() {
        return this.name;
    }

    /**
     * Overrides the default method to compare the name attributes.
     * @param o Object to be compared to
     * @return true if o is of Goods class and has the same name
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || o.getClass() != this.getClass()) {
            return false;
        }
        Goods oGoods = (Goods) o;

        return this.name.equals(oGoods.getName());
    }

    /**
     * Overrides the default method to return the name attribute hashCode.
     * @return name String attribute hashCode
     */
    @Override
    public int hashCode() {
        return this.name.hashCode();
    }
}
