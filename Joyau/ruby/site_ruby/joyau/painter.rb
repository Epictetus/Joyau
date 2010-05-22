require 'joyau/color'
require 'enumerator'

class Joyau::Painter
  # :nodoc:
  DEFAULT_OPTIONS = {
    :color => Joyau::Color::WHITE
  }

  def parse_options(options) # :nodoc:
    DEFAULT_OPTIONS.merge(options)
  end

  # Draws a circle.
  #
  # Valid options are :fill or :filled (defaulted to false) and :color.
  def circle(x, y, radius, options = {})
    options = parse_options(options)

    if options[:fill] or options[:filled]
      drawFillCircle(x, y, radius, options[:color])
    else
      drawCircle(x, y, radius, options[:color])
    end
  end

  # Draws a line.
  #
  # The only valid options is :color.
  def line(x1, y1, x2, y2, options = {})
    options = parse_options(options)
    drawLine(x1, y1, x2, y2, options[:color])
  end
  
  # Draws a rect.
  #
  # Valid options are :fill or :filled (defaulted to false) and :color.
  def rect(x1, y1, x2, y2, options = {})
    options = parse_options(options)

    if options[:filled] or options[:fill]
      drawFillRect(x1, y1, x2, y2, options[:color])
    else
      drawRect(x1, y1, x2, y2, options[:color])
    end
  end

  # Draws a pixel.
  #
  # The only valid option is :color.
  def pixel(x, y, options = {})
    rect(x, y, x, y, options)
  end

  def ary2coords(array) # :nodoc:
    coords = []

    array.each do |point|
      if point.is_a? Joyau::Point
        coords << point.x << point.y
      elsif point.is_a? Array
        coords << point[0] << point[1]
      else
        raise ArgumentError, "'points` should contain arrays or points"
      end
    end
    
    return coords
  end

  # Join all the given points by lines. If :closed is set to true,
  # the first and the last point will be joined.
  #
  # Points can be either Joyau::Point or Array:
  #   Joyau::Point.new(10, 30)
  #   [10, 30]
  def polyline(points, options = {})
    options = parse_options(options)

    points.each_cons(2) do |*sub_points|
      sub_points = sub_points[0] unless sub_points.size == 2
      coords = ary2coords(sub_points)
      x1, y1, x2, y2 = coords
      line(x1, y1, x2, y2, options)
    end

    if options[:closed] or options[:close]
      coords = ary2coords([points.first, points.last])
      x1, y1, x2, y2 = coords
      line(x1, y1, x2, y2, options)
    end
  end
end

class Joyau::Buffer
  # Paints a buffer (instance evaluating a painter for this
  # buffer).
  #
  # Returns itself.
  def paint(&block)
    Joyau.draw(:buffer => self, :painter => true) do |painter|
      painter.instance_eval(&block)
    end

    return self
  end
end

class Joyau::Sprite
  # Paints a copy of the sprite's buffer.
  def paint(&block)
    to_buf.paint(&block)
  end

  # Paints the sprite's buffer.
  # Dangerous like to_buf!.
  def paint!(&block)
    to_buf!.paint(&block)
  end
end

module Joyau
  # Paints the default buffer.
  def self.paint(&block)
    Joyau::Buffer.default.paint(&block)
  end
end
