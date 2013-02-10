package rpgtoolkit.editor.board;

import java.awt.Rectangle;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.Vector;

/**
 *
 * @author Joshua Michael Daly
 */
public class MultiLayerContainer 
{
    private ArrayList<BoardLayer> layers;
    protected Rectangle bounds;          // in tiles

    /**
     * Default constructor.
     */
    public MultiLayerContainer()
    {
        layers = new ArrayList();
        bounds = new Rectangle();
    }

    /**
     * Construct a MultilayerPlane to the specified dimensions.
     *
     * @param width
     * @param height
     */
    public MultiLayerContainer (int width, int height) 
    {
        bounds.width = width;
        bounds.height = height;
    }

    /**
     * Returns the total number of layers.
     *
     * @return the size of the layer vector
     */
    public int getTotalLayers() 
    {
        return layers.size();
    }

    /**
     * Changes the bounds of this plane to include all layers completely.
     */
    public void fitBoundsToLayers() 
    {
        int width = 0;
        int height = 0;

        Rectangle layerBounds = new Rectangle();

        for (int i = 0; i < layers.size(); i++) 
	{
            	getLayer(i).getBounds(layerBounds);

 		if (width < layerBounds.width)
		{ 
			width = layerBounds.width;
		}

            	if (height < layerBounds.height)
		{ 
			height = layerBounds.height;
		}
        }

        bounds.width = width;
        bounds.height = height;
    }

    /**
     * Returns a <code>Rectangle</code> representing the maximum bounds in
     * tiles.
     * @return a new rectangle containing the maximum bounds of this plane
     */
    public Rectangle getBounds() 
    {
        return new Rectangle(bounds);
    }

    /**
     * Adds a layer to the map.
     *
     * @param layer The {@link MapLayer} to add
     * @return the layer passed to the function
     */
    public BoardLayer addLayer(BoardLayer layer) 
    {
        layers.add(layer);
        return layer;
    }

    /**
     * Adds the MapLayer <code>l</code> after the MapLayer <code>after</code>.
     *
     * @param l the layer to add
     * @param after specifies the layer to add <code>l</code> after
     */
    public void addLayerAfter(BoardLayer l, BoardLayer after) 
    {
        layers.add(layers.indexOf(after) + 1, l);
    }

    /**
     * Add a layer at the specified index, which should be within
     * the valid range.
     *
     * @param index the position at which to add the layer
     * @param layer the layer to add
     */
    public void addLayer(int index, BoardLayer layer) 
    {
        layers.add(index, layer);
    }

    public void setLayer(int index, BoardLayer layer) 
    {
        layers.set(index, layer);
    }

    /**
     * Adds all the layers in a given java.util.Collection.
     *
     * @param layers a collection of layers to add
     */
    public void addAllLayers(Collection<BoardLayer> layers) 
    {
        this.layers.addAll(layers);
    }

    /**
     * Removes the layer at the specified index. Layers above this layer will
     * move down to fill the gap.
     *
     * @param index the index of the layer to be removed
     * @return the layer that was removed from the list
     */
    public BoardLayer removeLayer(int index) 
    {
        return layers.remove(index);
    }

    /**
     * Removes all layers from the plane.
     */
//    public void removeAllLayers() 
//    {
//        layers.removeAllElements();
//    }

    /**
     * Returns the layer vector.
     *
     * @return Vector the layer vector
     */
    public ArrayList<BoardLayer> getLayerArrayList() 
    {
        return layers;
    }

    /**
     * Sets the layer vector to the given java.util.Vector.
     *
     * @param layers the new set of layers
     */
    public void setLayerArrayList(ArrayList<BoardLayer> layers) 
    {
        this.layers = layers;
    }

    /**
     * Moves the layer at <code>index</code> up one in the vector.
     *
     * @param index the index of the layer to swap up
     */
    public void swapLayerUp(int index) 
    {
        if (index + 1 == layers.size()) 
        {            
            throw new RuntimeException(
                    "Can't swap up when already at the top.");
        }

        BoardLayer hold = layers.get(index + 1);
        layers.set(index + 1, getLayer(index));
        layers.set(index, hold);
    }

    /**
     * Moves the layer at <code>index</code> down one in the vector.
     *
     * @param index the index of the layer to swap down
     */
    public void swapLayerDown(int index) 
    {
        if (index - 1 < 0) 
        {
            throw new RuntimeException(
                    "Can't swap down when already at the bottom.");
        }

        BoardLayer hold = layers.get(index - 1);
        layers.set(index - 1, getLayer(index));
        layers.set(index, hold);
    }

    /**
     * Merges the layer at <code>index</code> with the layer below it
     *
     * @see MapLayer#mergeOnto
     * @param index the index of the layer to merge down
     */
//    public void mergeLayerDown(int index) 
//    {
//        if (index - 1 < 0) 
//        {
//            throw new RuntimeException("Can't merge down bottom layer.");
//        }
//
//        // TODO: We're not accounting for different types of layers!!!
//        TileLayer ntl;
//        try {
//            ntl = (TileLayer) getLayer(index - 1).clone();
//        }
//        catch (CloneNotSupportedException e) {
//            e.printStackTrace();
//            return;
//        }
//
//        getLayer(index).mergeOnto(ntl);
//        setLayer(index - 1, ntl);
//        removeLayer(index);
//    }

    /**
     * Returns the layer at the specified vector index.
     *
     * @param i the index of the layer to return
     * @return the layer at the specified index, or null if the index is out of
     *         bounds
     */
    public BoardLayer getLayer(int i) 
    {
        try 
        {
            return layers.get(i);
        } 
        catch (ArrayIndexOutOfBoundsException e) 
        {
            
        }
        
        return null;
    }

    /**
     * Gets a listIterator of all layers.
     *
     * @return a listIterator
     */
    public ListIterator<BoardLayer> getLayers() 
    {
        return layers.listIterator();
    }

    /**
     * Resizes this plane. The (dx, dy) pair determines where the original
     * plane should be positioned on the new area. Only layers that exactly
     * match the bounds of the map are resized, any other layers are moved by
     * the given shift.
     *
     * @see MapLayer#resize
     *
     * @param width  The new width of the map.
     * @param height The new height of the map.
     * @param dx     The shift in x direction in tiles.
     * @param dy     The shift in y direction in tiles.
     */
//    public void resize(int width, int height, int dx, int dy) 
//    {
//        ListIterator itr = getLayers();
//        
//        while (itr.hasNext()) 
//        {
//            BoardLayer layer = (BoardLayer)itr.next();
//            
//            if (layer.bounds.equals(bounds)) 
//            {
//                layer.resize(width, height, dx, dy);
//            } 
//            else 
//            {
//                layer.setOffset(layer.bounds.x + dx, layer.bounds.y + dy);
//            }
//        }
//
//        bounds.width = width;
//        bounds.height = height;
//    }

    /**
     * Determines wether the point (x,y) falls within the plane.
     *
     * @param x
     * @param y
     * @return <code>true</code> if the point is within the plane,
     *         <code>false</code> otherwise
     */
    public boolean inBounds(int x, int y) 
    {
        return x >= 0 && y >= 0 && x < bounds.width && y < bounds.height;
    }

    public Iterator<BoardLayer> iterator() 
    {
        return layers.iterator();
    }
}
