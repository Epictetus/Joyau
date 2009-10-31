if false # Joyau's documentation should not be included
  #
  # Joyau is a module containing classes in order to program in Ruby
  # for PSP. It aims to be easy to use.
  #
  module Joyau

    #
    # A class representing a position using three floats. It's used
    # for audio spatialisation.
    #
    class Vector3f

      #
      # Creates a new Vector3f.
      # If three arguments are given, then they should be given in the following
      # order : x, y, and z. The vector's position is set to that point.
      # If no arguments are given, then Vector3f.new(0, 0, 0) is returned.
      #
      def initialize(*args); end

      #
      # Sets the vector's x position.
      #
      def x=(val); end

      #
      # Sets the vector's y position.
      #
      def y=(val); end

      #
      # Sets the vector's z position.
      #
      def z=(val); end

      #
      # Returns the vector's x position.
      #
      def x; end

      #
      # Returns the vector's y position.
      #
      def y; end

      #
      # Returns the vector's z position.
      #
      def z; end

      #
      # Returns a new Vector3f whose position equals the sum of self's position
      # and op's position. Example :
      #
      #   Vector3f.new(10, 15, 30) + Vector3f.new(16, 30, 25)
      #   # returns Vector3f.new(26, 45, 55)
      #
      def +(op); end

      #
      # Returns a new Vector3f whose position equals the substraction of self's
      # position and op's position. Example :
      #
      #   Vector3f.new(10, 15, 30) - Vector3f.new(16, 30, 25)
      #   # returns Vector3f.new(-6, -15, 5)
      #
      def -(op); end

      #
      # Returns whether the x, y, and z positions of two vectors are equals.
      #
      def ==(op); end
    end

    #
    # Base class for all the sounds played through OpenAL.
    #
    class AudioObject

      #
      # Sets the sound's position to (x, y, z).
      #
      def setPos(x, y, z); end

      #
      # Sets the sound's direction to (x, y, z).
      #
      def setDirection(x, y, z); end

      #
      # Sets the sound's velocity to (x, y, z).
      #
      def setVelocity(x, y, z); end

      #
      # Same as setPos, but with a Vector3f.
      #
      #  obj.pos = Vector3f.new(0, 10, -4)
      #
      def pos=(vector); end

      #
      # Same as setDirection, but with a Vector3f.
      #
      def direction=(vector); end

      #
      # Same as setVelocity but with a Vector3f.
      #
      def velocity=(vector); end

      #
      # Returns whether the sound is playing.
      #
      def playing?; end
    end

    #
    # A class for playing wav sounds. They should be short. For long sound,
    # consider using an OGG stream.
    #
    class Sound < AudioObject
      #
      # Creates a new Sound. If one argument is given, then a sound is loaded.
      #
      #   sound = Sound.new("sound.wav")
      #
      def initialize(*args); end

      #
      # Loads a Wav sound, in the file called <em>filename</em>.
      #
      #   sound.loadWav("sound.wav")
      #
      def loadWav(filename); end

      #
      # Plays the sound.
      #
      def play; end

      #
      # Pauses the sound.
      #
      def pause; end

      #
      # Stops the sound.
      #
      def stop; end

      alias :load_wav :loadWav
    end

    #
    # A class which plays OGG files through a stream. It should be used
    # for long sounds.
    #
    # When using it, you have to call play and update as often as possible
    # Example :
    #
    #   stream = Joyau::Stream.new("music.ogg")
    #   while Joyau::mayPlay
    #     stream.update
    #     stream.play
    #   end
    #
    class Stream < AudioObject
      #
      # Creates a new Stream. If one argument is given, a file is loaded :
      #
      #   stream = Stream.new("music.ogg")
      #
      def initialize(*args); end

      #
      # Loads an ogg file called <em>filename</em>.
      #
      def loadOgg(filename); end

      #
      # Updates the stream. Should be called as often as possible.
      #
      def update; end

      #
      # Plays the stream. Should be called as often as possible.
      #
      def play; end

      #
      # pauses the stream.
      #
      def pause; end

      #
      # Stops the stream.
      #
      def stop; end

      alias :load_ogg :loadOgg
    end

    #
    # While sounds have their positions, you, the one who listen to them,
    # have another one. This module allows to modifiy that position.
    #
    module Listener
      #
      # Sets the listener's position to (x, y, z).
      #
      def setPos(x, y, z); end

      #
      # Sets the listener's direction to (x, y, z).
      #
      def setDirection(x, y, z); end

      #
      # Sets the listener's velocity to (x, y, z).
      #
      def setVelocity(x, y, z); end

      #
      # Same as setPos, but with a Vector3f.
      #
      #  obj.pos = Vector3f.new(0, 10, -4)
      #
      def pos=(vector); end

      #
      # Same as setDirection, but with a Vector3f.
      #
      def direction=(vector); end

      #
      # Same as setVelocity, but with a Vector3f.
      #
      def velocity=(vector); end

      #
      # Chanegs the listener's orientation.
      #
      def setOrientation(atX, atY, at2, upX, upY, up2); end
    end

    #
    # Inits Joyau's audio module.
    #
    def self.initAudio; end

    #
    # Stops Joyau's audio module.
    #
    def self.stopAudio; end

    #
    # When manipulating 2D coordinates, we need two values : x and y.
    # We can use them directly, but we might prefer using only one object.
    #
    class Point
      # Creates a new point.
      def initialize(x, y); end

      #
      # Returns the point's x value.
      #
      def x; end

      #
      # Returns the point's y value.
      #
      def y; end

      #
      # Sets the point's x value.
      #
      def x=(val); end

      #
      # Sets the point's y value.
      #
      def y=(val); end

      #
      # Returns a point whose position (x, y) equals to
      # (self.x + op.x, self.y + op.y).
      #
      def +(op); end

      #
      # Returns a point whose position (x, y) equals to
      # (self.x - op.x, self.y - op.y).
      #
      def -(op); end

      #
      # Returns whether the x and y values of two points are equals.
      #
      def ==(op); end
    end

    #
    # Often, we'll need to represent a rectangle, for example, when
    # determining collision. This class allows that.
    #
    class Rect
      # Creates a new Rect.
      def initialize(x, y, w, h); end
      #
      # Returns the rect's x value.
      #
      def x; end

      #
      # Returns the rect's y value.
      #
      def y; end

      #
      # Sets the rect's x value.
      #
      def x=(val); end

      #
      # Sets the rect's y value.
      #
      def y=(val); end

      #
      # Returns the rect's width.
      #
      def w; end

      #
      # Returns the rect's height.
      #
      def h; end

      #
      # Sets the rect's width.
      #
      def w=(val); end

      #
      # Sets the rect's height.
      #
      def h=(val); end
    end

    #
    # Base class of all the objects which are drawn on the screen.
    #
    class Drawable
      #
      # Returns the Rect in which all points (should) collide with the drawable.
      #
      def boundingRect; end

      #
      # Returns whether self collides with another Drawable.
      #
      def collide(obj); end

      #
      # Returns whether the point (x, y) is on self
      #
      def isOn(x, y); end

      #
      # Returns the drawable's x value.
      #
      def getX; end

      #
      # Returns the drawable's y value.
      #
      def getY; end

      #
      # Sets the drawable's x value.
      #
      def x=(val); end

      #
      # Sets the drawable's y value.
      #
      def y=(val); end

      #
      # Returns of how many pixels the drawable has been moved
      # horizontally since the last call to clearMove.
      #
      # Notice that in the following example :
      #
      #   drawable.x = 3
      #   drawable.clearMove
      #   drawable.x = 7
      #   drawable.x = 4
      #   val = drawable.movedX
      #
      # <em>val</em> is equal to 1.
      def movedX; end

      #
      # Returns of how many pixels the drawable has been moved
      # vertically since the last call to clearMove.
      #
      def movedY; end

      #
      # Returns the drawable's width.
      #
      def getW; end

      #
      # Returns the drawable's height.
      #
      def getH; end

      #
      # Sets the drawable's position to (x, y)
      #
      def setPos(x, y); end

      #
      # Same as setPos, but with a Point.
      #
      def pos=(point); end

      #
      # Moves the drawable of x and y pixels.
      #
      def move(x, y); end

      #
      # Cancels all the move done since the last call to clearMove. Equivalant
      # to :
      #
      #   obj.move(-obj.movedX, -obj.movedY)
      #
      def cancelMove; end

      #
      # Resets movedX and movedY to 0.
      #
      def clearMove; end

      #
      # Draws the drawable. Sometimes, clearMove is automatically called in
      # that method.
      #
      def draw; end

      alias :x :getX
      alias :y :getY
      alias :w :getW
      alias :h :getH

      alias :collide? :collide
      alias :is_on? :isOn
      alias :bounding_rect :boundingRect
      alias :moved_x :movedX
      alias :moved_y :moved_y
      alias :cancel_move :cancelMove
      alias :clear_move :clearMove
    end

    # A Shape is a Drawable which can be colorized.
    class Shape < Drawable
      #
      # Sets the used color.
      #
      # If only one argument is given, it has to be a hash, or at least
      # a color object. If three arguments are given, a RGB color is created.
      # If we have a fourth argument, its alpha value is set too.
      #
      def setColor(*args); end

      #
      # Sets <em>colorsNumber</em> colors in a gradient. They're given
      # in an array.
      #
      def setGradient(array); end
    
      # Returns an array containing all the colors.
      def getColors; end

      # Returns the first color.
      def getColor; end

      # Returns how many colors we can find in the Shape.
      def getColorsNumber; end

      alias :color= :setColor
      alias :gradient= :setGradient
      alias :colors :getColors
      alias :color :getColor
      alias :colorsNumber :getColorsNumber
    end

    # A FillableShape is a Shape which can be, either filled or not filled.
    class FillableShape < Shape
      # Toggles the filled state.
      def toggleFilled; end 
      
      # Returns whether the Shape is filled. 
      def filled; end

      # Sets the filled state.
      def filled=(val); end
    end

    #
    # Allows to draw picture on the screen.
    # Notice than the same picture will not be loaded this, so
    # don't worry about that.
    #
    class Sprite < Drawable
      # Creates a new Sprite, and load the picture <em>filename</em>.
      def initialize(filename); end

      # Loads the picture <em>pic</em>.
      def setPicture(pic); end

      # Returns the picture's name.
      def picture; end

      # Returns the sprite's angle.
      def getAngle; end

      # Sets the sprite's angle.
      def setAngle(val); end

      # Zooms on the sprite.
      def zoom(n); end

      # Get sprite's zoom level.
      def getZoom; end

      # Returns the sprite's alpha value.
      def getAlpha; end
      
      # Sets the sprite's alpha value.
      def setAlpha; end

      # Saves the sprite in a file.
      def saveFile(filename); end

      # Sets the sprite's direction.
      def setDirection(dir); end

      # Returns the sprite's direction.
      def getDirection; end
      
      # Sets how many steos there are horizontally and vertically.
      def setAnim(x_anim, y_anim); end

      # Sets how many frames there are between two animation's step.
      def setAnimTime(frame); end

      # Sets the sprite's rect which will be drawed.
      def setTile(x, y, w, h); end

      # Disables tiling on the Sprite.
      def unTile; end

      # Sets autoDir. When true, the Sprite guess in which direction it should
      # go.
      def autoDir=(val); end

      DOWN       = 0
      LEFT       = 1
      RIGHT      = 2
      UP         = 3
      UP_LEFT    = 4
      UP_RIGHT   = 5
      DOWN_LEFT  = 6
      DOWN_RIGHT = 7

      alias :angle :getAngle
      alias :angle= :setAngle
      alias :alpha :getAlpha
      alias :alpha= :setAlpha
      alias :direction :getDirection
      alias :direction= :setDirection
      alias :dir :getDirection
      alias :dir= :setDirection
      alias :picture= :setPicture
    end

    #
    # Circles can be drawn through this class. They're determined by their
    # center, and their radius.
    #
    class Circle < FillableShape
      #
      # Changes the radius value.
      #
      def setRadius(r); end

      #
      # Sets the Circle's center to a new point (x, y).
      #
      def setCenter(x, y); end

      #
      # Same as setCenter, but a Point is given as argument.
      #
      def center=(point); end

      #
      # Returns the circle's center as a Point.
      #
      def center; end

      #
      # Returns the center's x position.
      #
      def getCenterX; end

      #
      # Returns the center's y position.
      #
      def getCenterY; end

      #
      # Returns the circle's radius.
      #
      def getRadius; end

      alias :radius= :setRadius
      alias :centerX :getCenterX
      alias :centerY :getCenterY
      alias :center_x :getCenterX
      alias :center_y :getCenterY
      alias :radius :getRadius
    end

    #
    # If rectangles are represented via Rect, they cannot be drawn through
    # it. For that, you can use DrawableRect.
    #
    class DrawableRect < FillableShape
      #
      # Sets the rectangle's width.
      #
      def w=(val); end

      #
      # Sets the rectangle's height.
      #
      def h=(val); end

      #
      # Resizes the rectangle. Equivalant to :
      #
      #   rect.w = w
      #   rect.h = h
      #
      def resize(w, h); end

      #
      # Sets the rectangle's second corner.
      #
      def setCorner(x, y); end

      #
      # Same as setCorner, but with a Point.
      #
      def corner=(point); end

      #
      # Returns the rectangle's second corner.
      #
      def getCorner; end

      alias :corner :getCorner
    end

    #
    # A menu where icons are drawn in a circular disposition.
    # In the file from which the menu is created, icons should be disposed
    # vertically.
    #
    class CircleMenu < Drawable
      #
      # Loads a new menu. It's picture file is called <em>filename</em>,
      # we can found <em>nbIcons</em> on it, whose size is <em>w</em> *
      # <em>h</em> on the file, but the menu size is <em>iconW</em> *
      # <em>iconH</em> when drawn. This menu's distance is <em>dist</em>.
      #
      def load(filename, nbIcons, dist, w, h, x, y, iconW, iconH); end

      #
      # Changes the selected item. Give 1 for the left, and 0 for the right
      # direction.
      #
      def changeSelection(dir); end

      #
      # Returns the menu index.
      #
      def getIndex; end

      #
      # Changes the menu's alpha value to <em>alpha</em>.
      #
      def setAlpha(alpha); end

      #
      # Changes the menu's distance to <em>dist</em>.
      #
      def setDist(dist); end

      #
      # Changes the menu's angle to <em>angle</em>.
      #
      def setAngle(angle); end

      #
      # Returns the menu's alpha value.
      #
      def alpha; end

      #
      # Returns the menu's angle.
      #
      def angle; end

      #
      # Returns the menu's distance.
      #
      def dist; end

      alias :index :getIndex
      alias :alpha= :setAlpha
      alias :dist= :setDist
      alias :angle= :setAngle
      alias :change_selection :changeSelection
    end

    #
    # Allows to draw text, using an object instead of module's function.
    #
    class DrawableText < Shape
      #
      # Sets the object font. Its format should be oft. ( OSlib specific )
      #
      def setFont(f); end

      #
      # Sets the text's background color.
      #
      def setBackground(color); end

      #
      # Toggles the scripted value. When it's true, you can use symbol like
      # \\n, ... in the text.
      #
      def toggleScripted; end

      #
      # Toggles the stirring value.
      #
      def toggleStirring; end

      #
      # Changes the text which will be drawn on the screen by this object.
      #
      def setText(str); end

      #
      # Returns whether the text is scripted.
      #
      def scripted; end

      #
      # Returns whether the text is stirring.
      #
      def stirring; end

      #
      # Sets the stirring value.
      #
      def stirring=(val); end

      #
      # Returns the background's color.
      #
      def background; end

      #
      # Returns the font's name.
      #
      def font; end

      #
      # Returns the text.
      #
      def text; end

      alias :font= :setFont
      alias :background= :setBackground
      alias :text= :setText
    end

    #
    # We can use OFT, but we might prefer using PSP's font. This modules
    # contains constant for that use.
    #
    module Intrafont
      ADVANCE_H      = 0x00000000
      ADVANCE_V      = 0x00000100

      ALIGN_LEFT     = 0x00000000
      ALIGN_CENTER   = 0x00000200
      ALIGN_RIGHT    = 0x00000400
      ALIGN_FULL     = 0x00000600

      SCROLL_LEFT    = 0x00002000
      SCROLL_SEESAW  = 0x00002200
      SCROLL_RIGHT   = 0x00002400
      SCROLL_THROUGH = 0x00002600

      WIDTH_VAR      = 0x00000000
      WIDTH_FIX      = 0x00000800

      ACTIVE         = 0x00001000

      CACHE_MED      = 0x00000000
      CACHE_LARGE    = 0x00004000
      CACHE_ASCII    = 0x00008000
      CACHE_ALL      = 0x0000C000

      STRING_ASCII   = (0x00010000*0x00)
      STRING_CP437   = (0x00010000*0x01)
      STRING_CP850   = (0x00010000*0x05)
      STRING_CP866   = (0x00010000*0x0b)
      STRING_SJIS    = (0x00010000*0x0d)
      STRING_GBK     = (0x00010000*0x0e)
      STRING_KOR     = (0x00010000*0x0f)
      STRING_BIG5    = (0x00010000*0x10)
      STRING_CP1251  = (0x00010000*0x12)
      STRING_CP1252  = (0x00010000*0x13)
      STRING_UTF8    = (0x00010000*0xff)

      #
      # Inits the Intrafont module
      #
      def self.init; end

      #
      # Stops the Intrafont module.
      #
      def self.stop; end
    end

    #
    # Like DrawableText, but for Intrafont.
    #
    class IntraText < Drawable
      # Sets the text which will be drawn by self.
      def text=(val); end

      # Returns the text which will be drawn by self.
      def text; end

      #
      # Loads a new font, from the file called <em>filename</em> (usually in
      # flash0), and some options which can be found in Intrafont's constants.
      #
      def load(filename, options); end

      # Activates the font. Should be called before drawing the text.
      def activate; end

      #
      # Sets the font's style.
      # <em>size</em> is the scalling applied to the font.
      # <em>color</em> is its color's hexadecimal value.
      # <em>shadow</em> is the hexadecimal value of the shadow's color.
      # <em>options</em> are some of Intrafont's constants.
      #
      def setStyle(size, color, shadow, options); end

      # Sets the encoding, which is defined as a constant in Intrafont.
      def encoding=; end

      # Sets the font which will be used if a character isn't found in that font.
      def altFont=(font); end
    end

    #
    # When using tiles, we don't have to check collision with the full object.
    # Sometimes, only some of the sides should be checked. CollisionType
    # tells to a GameMap where should be checked the collisions.
    #
    class CollisionType
      # Returns whether collision are checked for the left side.
      def left; end

      # Returns whether collision are checked for the right side.
      def right; end

      # Returns whether collision are checked for the upper side.
      def up; end

      # Returns whether collision are checked for the lower side.
      def down; end

      # Returns whether collision are checked inside the tile.
      def content; end

      # Sets whether collision should be checked for the left side.
      def left=(val); end

      # Sets whether collision should be checked for the right side.
      def right=(val); end

      # Sets whether collision should be checked for the upper side.
      def up=(val); end

      # Sets whether collision should be checked for the lower side.
      def down=(val); end

      # Sets whether collision should be checked inside the tile.
      def content=(val); end

      #
      # When 5 arguments are given, left, right, up, down, and content
      # are respectively set.
      #
      def initialize(*args); end
    end

    # A GameMap contains tiles. This is the class which represents these.
    class Tile
      # Returns the tile's x position
      def x; end

      # Returns the tile's y position
      def y; end

      # Returns the tile's tileset id.
      def tileset; end

      # Returns the tile's x position on its tileset.
      def tileX; end

      # Returns a CollisionType telling where collisions should be checked.
      def type; end

      # Sets the tile's x position
      def x=(val); end

      # Sets the tile's y position
      def y=(val); end

      # Sets the tile's tileset id.
      def tileset=(val); end

      # Sets the tile's x position on its tileset.
      def tileX=(val); end

      # Sets a CollisionType telling where collisions should be checked.
      def type=(val); end

      #
      # When 5 arguments are given, tileset, tileX, tileY, x, and y are
      # respectively set. If a 6th one is given, then type is also set.
      #
      def initialize(*args); end
    end

    #
    # Allows to draw alot of tiles from some tilesets, to check collision between
    # them and another Drawable, and to move them easily.
    #
    class GameMap < Drawable
      # Tilesets are given as argument. You may either give one array, or 
      # seperated arguments.
      def initialize(*args); end

      # Adds the tileset <em>filename</em> to the GameMap.
      def addTileset(str); end

      # Changes the tiles' size.
      def setTileSize(w, h); end

      # Returns the tiles width.
      def tileWidth; end

      # Returns the tiles height.
      def tileHeight; end

      #
      # Changes collisionH. When different to -1, collision are only checked
      # for the <em>collisionH</em> lower pixels of the drawables.
      #
      def collisionH=(n); end

      # Returns collisionH's value.
      def collisionH; end

      # Returns an absolute position from a relative position as a Point.
      def absToRel(x, y); end

      # Returns a relative position from an absolute position as a Point.
      def absToRel(x, y); end

      # Centers the map on the point (x, y)
      def centerOn(x, y); end

      #
      # Adds one or more elements to the GameMap. When five arguments are given,
      # a new Tile is created. If not, the first argument is given to <<.
      #
      def addElem(*args); end

      #
      # If obj is an Array, all its elements are put in the map. If it's a Tile,
      # that tile is inserted in self.
      #
      # self is returned.
      #
      def <<(obj); end

      # Clears the map's tiles and tilesets.
      def clear; end

      # Clears the map's tiles.
      def clearTiles; end

      #
      # Returns an array containing all the tiles. Deleting or inserting
      # items in it won't affect self, but modifying them will have an impact
      # on the map's tiles.
      #
      def tiles; end

      #
      # Returns an array containing all the tilesets as Sprite. This array works
      # like the one returned by tiles.
      #
      def tilesets; end

      #
      # Yields each tile of the map. Notice you may modifiy it. Example :
      #
      #   map.each_tile { |tile| tile.tileset += 1 }
      #
      def each_tile # :yield:
      end

      #
      # Yields each tileset of the map. As for each_tile, you may modifiy the
      # yielded object.
      #
      def each_tileset # :yield:
      end

      #
      # Removes from the map all the object which match with a condition,
      # like Array#reject!. Example :
      #
      #   map.reject_tiles { |tile| tile.x > 100 }
      #
      def reject_tiles # :yield:
      end

      COL_FULL       = CollisionType.new(true , false, false, false, false)
      COL_LEFT       = CollisionType.new(false, true , false, false, false)
      COL_RIGHT      = CollisionType.new(false, false, true , false, false)
      COL_UP         = CollisionType.new(false, false, false, true , false)
      COL_DOWN       = CollisionType.new(false, false, false, false, true )
      COL_NO         = CollisionType.new(false, false, false, false, false)
      COL_LEFT_RIGHT = CollisionType.new(false, true , true , false, false)
      COL_LEFT_UP    = CollisionType.new(false, true , false, true , false)
      COL_LEFT_DOWN  = CollisionType.new(false, true , false, false, true )
      COL_RIGHT_UP   = CollisionType.new(false, false, true , true , false)
      COL_RIGHT_DOWN = CollisionType.new(false, false, true , false, true )
      COL_UP_DOWN    = CollisionType.new(false, false, false, true , true )

      alias :add_tileset :addTileset
      alias :set_tilie_size :setTileSize
      alias :tile_width :tileWidth
      alias :tileHeight :tileHeight
      alias :collision_h :collisionH
      alias :collision_h= :collisionH=
      alias :absToRel :abs2rel
      alias :relToAbs :rel2abs
      alias :center_on :centerOn
      alias :add_elem :addElem
      alias :clearTiles :clear_tiles
    end

    # Enables auto swizzling for all the pictures which will be loaded.
    def self.swizzle; end

    #
    # Returns true unless the user has already exited via the home menu. It
    # should be the condition of all your main loops. Example :
    #
    #   Joyau::initLib
    #   Joyau::initGfx
    #
    #   skip = false
    #   while Joyau::mayPlay
    #     # do something
    #
    #     unless skip
    #       Joyau::startDraw
    #       # draw something
    #       Joyau::endDraw
    #     end
    #
    #     skip = Joyau::sync
    #   end
    #
    #   Joyau::stopGfx
    #   Joyau::stopLib
    #
    def self.mayPlay; end

    # Allows to draw on the screen.
    def self.startDraw; end

    # Stops drawing on the screen.
    def self.endDraw; end

    # Updates the screen. Returns true if a fram has been skiped.
    def self.sync; end

    #
    # Sets the minimum and the maximum frame skiped. By default, it's set
    # so that there'll be 60 frames per second.
    #
    def self.frameskip(min, max); end

    # Returns a new color as a Hash.
    def self.color(r, g, b, a = 255); end

    # Draws a line, in <em>col</em>, from (x1, y1) to (x2, y2). 
    def self.drawLine(x1, y1, x2, y2, col)
    end

    # Draws a rect, in <em>col</em>, from (x1, y1) to (x2, y2).
    def self.drawRect(x1, y1, x2, y2, col)
    end

    # Draws a filled rect, in <em>col</em>, from (x1, y1) to (x2, y2).
    def self.drawFillRect(x1, y1, x2, y2, col)
    end

    # Draws a triangle, in three colors.
    def self.drawTriangle(x1, y1, x2, y2, x3, y3, col1, col2, col3)
    end

    #
    # Draws a circle whose center is at (x, y), whose radius is <em>rad</em>,
    # and whose color is <em>col</em>.
    #
    def self.drawCircle(x, y, rad, col)
    end

    # Like drawCircle, but the circle is filled.
    def self.drawFillCircle(x, y, rad, col)
    end

    # Takes a screenshot, and save it in <em>file</em>
    def self.screenshot(file)
    end

    # Fades the screen.
    def self.fade
    end

    # Loads and set the used font.
    def self.setFont(name)
    end

    # Returns the length of str, if draw with the actual font.
    def self.getLength(str)
    end

    # Sets the text's background.
    def self.setTextBackground(col)
    end

    # Sets the text's color.
    def self.setTextColor(col)
    end

    # Draws the string <em>txt</em>, at (x, y).
    def self.drawText(x, y, txt)
    end

    # Same as drawText, but you can use characters like \\n.
    def self.drawScripted(x, y, txt); end

    # Same as drawtext, but the text is stirring.
    def self.drawStirringText(x, y, txt); end

    # Clears the screen.
    def self.clearScreen; end

    # Inits Joyau's graphics module;
    def self.initGfx; end

    # Inits the main library used by Joyau.
    def self.initLib; end

    # Stops the main library used by Joyau.
    def self.stopLib; end

    # Stops Joyau's graphics module.
    def self.stopGfx; end

    # Sets the current working directory to <em>dir</em>.
    def self.cd(dir); end

    # Returns the actual timestamp.
    def self.timestamp; end

    # Returns how many minutes are left for the battery.
    def self.powerTime; end

    # Returns the batter power percentage.
    def self.powerPercent; end

    # Creates the dir <em>dir</em>.
    def self.mkdir(dir); end

    # Removes the directory <em>dir</em>.
    def self.rmdir(dir); end

    # Removes the file <em>file</em>.
    def self.rm(file); end

    # Moves the file <em>old</em> to <em>new</em>
    def self.mv(old, new); end

    #
    # Fills a hash containing informations about the keys.
    # This is deprecated, use Pad instead.
    #
    def self.readKeys; end

    # Returns a String, entered via the OSK.
    def self.gets; end

    # Clears all the loaded images.
    def self.clearImages; end

    # Clears all the loaded font.
    def self.clearFonts; end

    # Clears all the loaded sounds.
    def self.clearSounds; end

    # Writes str on a debuging dialog.
    def self.debug(str); end

    #
    # Puts all the given arguments on the psp debug's screen. Be sure
    # they're all strings.
    #
    def self.puts(*args); end

    # Exits the game.
    def self.exitGame; end

    # Module which allows to check pressed, held, and released keys.
    module Pad
      # Updates the checked values.
      def self.update; end

      #
      # Returns whether the key <em>key</em> is pressed, where key is
      # a constant or key name.
      #
      def self.held?(key); end

      # Returns whether a key has been pressed right now.
      def self.pressed?(key); end

      # Returns whether a key has been released right now.
      def self.released?(key); end

      # Returns the analogic stick's x position.
      def self.stickX; end
      
      # Returns the analogic stick's y position.
      def self.stickY; end

      SELECT   = 0x000001
      START    = 0x000008
      L        = 0x000100
      R        = 0x000200
      CROSS    = 0x004000
      SQUARE   = 0x008000
      TRIANGLE = 0x001000
      CIRCLE   = 0x002000
      UP       = 0x000010
      DOWN     = 0x000040
      LEFT     = 0x000080
      RIGHT    = 0x000020
      HOLD     = 0x020000
    end

    # A Cursor is a Sprite which follows the analogic stick.
    class Cursor < Sprite
      #
      # Updates the cursor's position, according to the analogic stick position.
      # Since this method calls Pad.update, you shouldn't call it after.
      #
      def updatePos; end

      #
      # Sets the cursor's sensibility. If the sensibility is higher, 
      # the cursor moves slower.
      #
      def setSensibility(sens); end

      # Returns the cursor sensibility
      def sensibility; end

      # Sets the rect the cursor may move in.
      def rect=(rect); end

      # Returns the rect the cursor may move in.
      def rect; end

      alias :sensibility= :setSensibility
      alias :update_pos :updatePos
    end

    # Allows to draw line, and to check collision with them with precision.
    class Line < Shape
      # Sets the line's second point.
      def setPoint(x, y); end

      # Like setPoint, but with a Point.
      def point=(point); end

      # Returns the line's second point.
      def getPoint; end

      alias :point :getPoint
      alias :set_point :setPoint
    end

    # Allows to draw a text inside a message box.
    class Message < Drawable
      # Sets the message's title.
      def setTitle(str); end

      # Sets the message's text.
      def setText(str); end

      #
      # Sets the message's image, which is drawed next to the text. This image
      # is given as a Sprite.
      #
      def setImage(sprite); end

      # Sets a Sprite as background for the text.
      def setBackground(sprite); end

      # Sets a color as background for the text.
      def setBgColor(color); end

      # Sets the title's color.
      def setTitleColor(color); end

      # Sets the text's color.
      def setTextColor(color); end

      # Sets the border's color.
      def setBorderColor(color); end

      # Sets the text's font, which is given as a filename.
      def setTextFont(fontname); end

      # Sets the title's font
      def setTitleFont(fontname); end

      # Sets the title's position, relatively to the message box.
      def setTitlePos(x, y); end

      # Same as setTitlePos, but with a Point.
      def titlePos=(point); end

      # Changes the message's width.
      def w=(width); end

      # Changes the message's height.
      def h=(height); end

      # Resizes the message.
      def resize(w, h); end
      
      # Returns the message's title.
      def title; end

      # Returns the message's text.
      def text; end

      # Returns the text's image.
      def setImage; end

      # Returns the text's background as a Sprite.
      def background; end

      # Returns the text's background as a color.
      def bgColor; end

      # Returns the title's color.
      def titleColor; end

      # Returns the text's color.
      def textColor; end

      # Returns the border's color.
      def borderColor; end

      # Returns the text's font as a filename.
      def textFont; end

      # Returns the title's font as a filename.
      def titleFont; end

      # Returns the title's position, relatively to the message box.
      def titlePos; end
      
      alias :title= :setTitle
      alias :text= :setText
      alias :image= :setImage
      alias :background= :setBackground
      alias :bgColor= :setBgColor
      alias :titleColor= :setTitleColor
      alias :textColor= :setTextColor
      alias :borderColor= :setBorderColor
      alias :textFont= :setTextFont
      alias :titleFont= :setTitleFont
    end

    # MsgSelecter uses MsgConfig in order to change a Message's appareance.
    class MsgConfig
      # Returns the message's image.
      def image; end

      # Sets the message's image.
      def image=(val); end

      # Returns the background.
      def bg; end

      # Sets the background.
      def bg=(val); end

      # Returns the title's color.
      def titleColor; end

      # Sets the title's color.
      def titleColor=(val); end

      # Returns the text's color.
      def textColor; end
      
      # Sets the text's color.
      def textColor=(val); end

      # Returns the border's color.
      def borderColor; end

      # Sets the border's color.
      def borderColor=(val); end

      # Returns the text's font.
      def textFont; end
      
      # Sets the text's font.
      def textFont=(val); end

      # Returns the title's font.
      def titleFont; end

      # Sets the title's font.
      def titleFont=(val); end
    end

    # Allows to select between multiple Messsage, disposed vertically.
    class VerticalMsgSelecter < Drawable
      PREV = -1
      NEXT =  1
      
      # Select the <em>index + dir</em>th element. 
      def select(dir); end

      # Inserts the object <em>obj</em> in the selecter.
      def <<(obj); end

      # Resizes the selecter.
      def resize(w, h); end
      
      # Returns the selected element.
      def selected; end

      # Returns the <em>n</em>th elemet.
      def item(n); end

      # Returns the selected element's index.
      def index; end

      # Returns the selecter's size.
      def size; end

      # Sets whether this selecter may have a focused element.
      def focus=(val); end
      
      # Returns whether this selecter may have a focused element.
      def focus; end

      #
      # Returns the configuration used for all the messages. You may
      # modify it.
      #
      def conf; end

      # Returns the configuration used for the focused message.
      def focusConf; end

      # Sets the configuration used for all the messages.
      def conf=(conf); end

      # Sets the configuration used for the focused message.
      def focusConf=(conf); end
    end

    # Allows to select between multiple Messsage, disposed horizontally.
    class HorizontalMsgSelecter < Drawable
      PREV = -1
      NEXT =  1
      
      # Select the <em>index + dir</em>th element. 
      def select(dir); end

      # Inserts the object <em>obj</em> in the selecter.
      def <<(obj); end

      # Resizes the selecter.
      def resize(w, h); end
      
      # Returns the selected element.
      def selected; end

      # Returns the <em>n</em>th elemet.
      def item(n); end

      # Returns the selected element's index.
      def index; end

      # Returns the selecter's size.
      def size; end

      # Sets whether this selecter may have a focused element.
      def focus=(val); end
      
      # Returns whether this selecter may have a focused element.
      def focus; end

      #
      # Returns the configuration used for all the messages. You may
      # modify it.
      #
      def conf; end

      # Returns the configuration used for the focused message.
      def focusConf; end

      # Sets the configuration used for all the messages.
      def conf=(conf); end

      # Sets the configuration used for the focused message.
      def focusConf=(conf); end
    end

    # Allows to select between multiple HorizontalMsgSelecter
    class MultiHorizontalMsgSelecter < Drawable
      PREV = -1
      NEXT =  1
      
      # Select the <em>index + dir</em>th element. 
      def select(dir); end

      # Inserts the object <em>obj</em> in the selecter.
      def <<(obj); end

      # Resizes the selecter.
      def resize(w, h); end
      
      # Returns the selected element.
      def selected; end

      # Returns the <em>n</em>th elemet.
      def item(n); end

      # Returns the selected element's index.
      def index; end

      # Returns the selecter's size.
      def size; end

      # Sets whether this selecter may have a focused element.
      def focus=(val); end
      
      # Returns whether this selecter may have a focused element.
      def focus; end
    end

    # Allows to select between multiple VerticalMsgSelecter
    class MultiVerticalMsgSelecter < Drawable
      PREV = -1
      NEXT =  1
      
      # Select the <em>index + dir</em>th element. 
      def select(dir); end

      # Inserts the object <em>obj</em> in the selecter.
      def <<(obj); end

      # Resizes the selecter.
      def resize(w, h); end
      
      # Returns the selected element.
      def selected; end

      # Returns the <em>n</em>th elemet.
      def item(n); end

      # Returns the selected element's index.
      def index; end

      # Returns the selecter's size.
      def size; end

      # Sets whether this selecter may have a focused element.
      def focus=(val); end
      
      # Returns whether this selecter may have a focused element.
      def focus; end
    end
    
    # A particles generator for Joyau.
    class Particles < Drawable
      # Sets the file containing the particles.
      def setFile(file); end

      #
      # Configurates the particles generator. A particle will live during
      # <em>time</em>, its speed is <em>speed</em>, but its minimum speed
      # is <em>mspeed</em>. The gravity is <em>gravity</em>.
      #
      def setParam(time, speed, gravity, mspeed); end
      
      # Adds a particles at (x, y).
      def addParticles(x, y); end
      
      # Returns each particle's lifetime.
      def getTime; end

      # Returns each particle's speed
      def getSpeed; end

      # Returns the gravity
      def getGravity; end

      # Returns each particle's minimum speed
      def getMinSpeed; end

      alias :file= :setFile
      alias :time :getTime
      alias :speed :getSpeed
      alias :gravity :getGravity
      alias :minSpeed :getMinSpeed
    end

    # Allows an image to scroll through the screen.
    class Scrolling < Drawable
      LEFT = Sprite::LEFT
      RIGHT = Sprite::RIGHT
      UP = Sprite::UP
      DOWN = Sprite::DOWN
    
      # Sets the image which should be loaded.
      def setSprite(filename); end

      # Chanegs the direction in which the Scrolling will scrolls.
      def setDir(dir); end

      # Cofngurates the amount of moved pixels at play's call.
      def setSpeed(val); end

      # Returns the direction
      def dir; end
      
      # Returns the speed
      def speed; end

      # Scrolls the Scrolling.
      def play; end

      alias :sprite= :setSprite
      alias :dir= :setDir
      alias :direction :dir
      alias :direction= :setDir
      alias :speed= :setSpeed
    end

    # Allow to count the elapsed seconds between two moments.
    class Timer
      # Resets the timer.
      def resset; end

      # Gets the elapsed seconds.
      def getTime; end

      # Pauses the timer.
      def pause; end
      
      # Resumes the timer.
      def resume; end

      # Returns whether the timer is paused.
      def paused; end

      alias :time :getTime
    end

    # Allows to establish an USB connection.
    module Usb
      # Inits the USB module.
      def self.init; end

      # Stops the USB module.
      def self.stop; end

      # Starts the USB connection.
      def connect; end

      # Ends the USB connection.
      def disconnect; end

      #
      # Returns a Hash containing the USB connexion's state.
      # Valid keys are "activated", "connected", and "established".
      #
      def state; end

      # Returns whether the USB connection is activated.
      def activated?; end

      # Returns whether the USB connection is established.
      def established?; end

      # Returns whether the USB cable is connected.
      def connected?; end
    end

    # Allows to draw a Triangle.
    class Triangle < Shape
      # Sets the point in which the triangle is.
      def setPoints(x1, y1, x2, y2, x3, y3); end

      # Returns an array containing the triangle's points.
      def getPoints; end

      alias :points :getPoints
    end
  end

  alias :may_play? :mayPlay
end
