#
# A class which represents a color. Used in order to have a better interface
# than a simple hash.
#
# It can be given to any method asking a color, excepted those which require
# an hexdecimal value, just like a hash :
#
#   line.color = Color.new
#
# However, Joyau's function which returns a color returns a hash. Luckily, You
# can easily create a Color from them :
#
#   a_color = Color.new(line.color)
#
# If you want to use it with intrafont, you'll need to get an hexadecimal value :
#
#   a_color = Color.new.hex
#
class Color
  # 
  # The hash, which allows the compatibility with Joyau.
  #
  attr_reader :hash

  #
  # Returns the red level in the color
  #
  def r
    @hash["r"]
  end

  #
  # Returns the green level in the color
  #
  def g
    @hash["g"]
  end

  #
  # Returns the blue level in the color
  #
  def b
    @hash["b"]
  end

  #
  # Returns the color's alpha value
  #
  def a
    @hash["a"]
  end

  #
  # Changes the color's red value.
  #
  def r=(val)
    @hash["r"] = val
  end

  #
  # Changes the color's green value
  #
  def g=(val)
    @hash["g"] = val
  end

  #
  # Changes the color's blue value
  #
  def b=(val)
    @hash["b"] = val
  end
  
  #
  # Changes the color's alpha value
  #
  def a=(val)
    @hash["a"] = val
  end

  #
  # Returns whether self and other are equal.
  # Example :
  #   
  #   require 'joyau/color'
  #
  #   Color.new(255, 0, 0, 255) == Color.new(255, 0, 0)
  #   => true
  #   Color.new(255, 0, 0, 255) == Color.new(255, 0, 0, 127)
  #   => false
  #
  def ==(other)
    @hash["r"] == other.r and
      @hash["g"] == other.g and
      @hash["b"] == other.b and
      @hash["a"] == other.a
  end

  #
  # Creates a new Color.
  # If one argument is given, it's a Hash :
  # Example :
  #
  #   Color.new(DrawableText.new.backgroud)
  #
  # If three arguments are given, a new color is created, using
  # the RGB format :
  #
  #   Color.new(0, 0, 0) # Black
  #   Color.new(255, 0, 0) # Red
  #   Color.new(0, 255, 0) # Green
  #   Color.new(255, 255, 255) # White
  #
  # If a fourth argument is given, it's the alpha value, which is 255 by
  # default :
  #
  #   Color.new(255, 255, 255, 127)
  #
  # Finally, if no arguments are given, a white color is created :
  #
  #   Color.new == Color.new(255, 255, 255, 255)
  #   => true
  #
  def initialize(*args)
    @hash = Hash.new

    if args.size == 1
      @hash = args[0]
    elsif args.size >= 3
      @hash["r"] = args[0]
      @hash["g"] = args[1]
      @hash["b"] = args[2]
      if args.size > 3
        @hash["a"] = args[3]
      else
        @hash["a"] = 255
      end
    else
      @hash["r"] = 255
      @hash["g"] = 255
      @hash["b"] = 255
      @hash["a"] = 255
    end
  end
  
  #
  # Returns a color's hexadecimal value :
  #
  #   Color.new(255, 255, 0).hex
  #   => 0xffffff00
  #
  def hex
    str = ""
    tmp = @hash["a"].to_s(16)
    if tmp.size == 1
      tmp = "0" + tmp
    end

    str += tmp
    
    tmp = @hash["r"].to_s(16)
    if tmp.size == 1
      tmp = "0" + tmp
    end

    str += tmp

    tmp = @hash["g"].to_s(16)
    if tmp.size == 1
      tmp = "0" + tmp
    end

    str += tmp

    tmp = @hash["b"].to_s(16)
    if tmp.size == 1
      tmp = "0" + tmp
    end

    str += tmp
    return str.to_i
  end

  alias_method :red, :r
  alias_method :green, :g
  alias_method :blue, :b
  alias_method :alpha, :a

  alias_method :red=, :r=
  alias_method :green=, :g=
  alias_method :blue=, :b=
  alias_method :alpha=, :a=
end
