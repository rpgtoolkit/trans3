package rpgtoolkit.common.editor.types;

import java.awt.Rectangle;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.ListIterator;
import rpgtoolkit.editor.board.BoardLayer;

/**
 * Note: Look back at Tiled's MultiLayerPlane, not every method that exists there
 * has been implemented here yet.
 * @author Joshua Michael Daly
 */
public interface MultiLayerContainer 
{
    /**
     * Returns the total number of layers.
     *
     * @return the size of the layer vector
     */
    public int getTotalLayers();

    /**
     * Changes the bounds of this plane to include all layers completely.
     */
    public void fitBoundsToLayers();

    /**
     * Returns a <code>Rectangle</code> representing the maximum bounds in
     * tiles.
     * @return a new rectangle containing the maximum bounds of this plane
     */
    public Rectangle getBounds();

    /**
     * Adds a layer to the map.
     *
     * @param layer The {@link MapLayer} to add
     * @return the layer passed to the function
     */
    public BoardLayer addLayer(BoardLayer layer);

    /**
     * Adds the MapLayer <code>l</code> after the MapLayer <code>after</code>.
     *
     * @param l the layer to add
     * @param after specifies the layer to add <code>l</code> after
     */
    public void addLayerAfter(BoardLayer l, BoardLayer after);

    /**
     * Add a layer at the specified index, which should be within
     * the valid range.
     *
     * @param index the position at which to add the layer
     * @param layer the layer to add
     */
    public void addLayer(int index, BoardLayer layer);

    public void setLayer(int index, BoardLayer layer);

    /**
     * Adds all the layers in a given java.util.Collection.
     *
     * @param layers a collection of layers to add
     */
    public void addAllLayers(Collection<BoardLayer> layers);

    /**
     * Removes the layer at the specified index. Layers above this layer will
     * move down to fill the gap.
     *
     * @param index the index of the layer to be removed
     * @return the layer that was removed from the list
     */
    public BoardLayer removeLayer(int index);

    /**
     * Returns the layer vector.
     *
     * @return Vector the layer vector
     */
    public ArrayList<BoardLayer> getLayerArrayList();

    /**
     * Sets the layer vector to the given java.util.Vector.
     *
     * @param layers the new set of layers
     */
    public void setLayerArrayList(ArrayList<BoardLayer> layers);

    /**
     * Moves the layer at <code>index</code> up one in the vector.
     *
     * @param index the index of the layer to swap up
     */
    public void swapLayerUp(int index);

    /**
     * Moves the layer at <code>index</code> down one in the vector.
     *
     * @param index the index of the layer to swap down
     */
    public void swapLayerDown(int index);

    /**
     * Returns the layer at the specified vector index.
     *
     * @param i the index of the layer to return
     * @return the layer at the specified index, or null if the index is out of
     *         bounds
     */
    public BoardLayer getLayer(int i);

    /**
     * Gets a listIterator of all layers.
     *
     * @return a listIterator
     */
    public ListIterator<BoardLayer> getLayers();

    /**
     * Determines whether the point (x,y) falls within the plane.
     *
     * @param x
     * @param y
     * @return <code>true</code> if the point is within the plane,
     *         <code>false</code> otherwise
     */
    public boolean inBounds(int x, int y);

    public Iterator<BoardLayer> iterator();
}
