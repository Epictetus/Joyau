#
# danzeff.rb - Another input method for Joyau
#

# :stopdoc:

#
# Copyright 2009 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:

require 'joyau/color'
require 'joyau/inspect'
require 'joyau/inherited'

module Joyau
  #
  # In the PSP's world, a well known input method is the Danzeff keyboard.
  # This class is not the same keyboard, but it allows to enter text the same
  # way.
  #
  # You can modifiy the characters displayed by the keyboard ( <em>pages</em> ),
  # The color used for the text ( <em>txt_color</em> ), when focused 
  # ( <em>:focus</em> ), or usually ( <em>:normal</em> ).
  #
  # You can also specify the background <em>bg_color</em>, which is either
  # a color or a sprite ( which will be redimensionned ).
  #
  # You may also change the <em>keys</em> used  for controlling this object.
  #
  class Danzeff < Drawable
    joyau_inherited

    # Creates a new Danzeff keyboard.
    def initialize # :nodoc:
      @pages = [ 
        [
          [ 'a', 'b', 'c', 'd' ],
          [ 'e', 'f', 'g', 'h' ],
          [ 'i', 'j', 'k', 'l' ],
          [ 'm', 'n', 'o', 'p' ],
          [ 'q', 'r', 's', 't' ],
          [ 'u', 'v', 'w', 'x' ],
          [ 'y', 'z', '0', '1' ],
          [ '2', '3', '4', '5' ],
          [ '6', '7', '8', '9' ]
        ],
        [
          [ 'A', 'B', 'C', 'D' ],
          [ 'E', 'F', 'G', 'H' ],
          [ 'I', 'J', 'K', 'L' ],
          [ 'M', 'N', 'O', 'P' ],
          [ 'Q', 'R', 'S', 'T' ],
          [ 'U', 'V', 'W', 'X' ],
          [ 'Y', 'Z', '0', '1' ],
          [ '2', '3', '4', '5' ],
          [ '6', '7', '8', '9' ]
        ],      
        [
          [ ' ',  -1, '(', ')' ],
          [ '[', ']', '{', '}' ],
          [ '@', '$', '+', '-' ],
          [ '*', '/', '%', '#' ],
          [ '"', '\'', '&', '|' ],
          [ '^', '~', '=', '>' ],
          [ '<', '!', '?', ',' ],
          [ '.', ';', ':', '_' ],
          [ '\\', '...', ' ', ' ' ]
        ]      
      ]

      @txt_colors = {
        :normal => Color::BLACK,
        :focus  => Color::BLACK
      }

      @bg_colors = {
        :normal => Color::WHITE,
        :focus  => Color.new(255, 255, 0)
      }

      @border_col = Color::WHITE

      @keys = {
        :next_page => Pad::R,
        :prev_page => Pad::L,
        :bottom    => Pad::CROSS,
        :up        => Pad::TRIANGLE,
        :left      => Pad::SQUARE,
        :right     => Pad::CIRCLE
      }

      @w, @h = 480, 272
      @page_id = 0

      @str = ""
    end

    def selected_id(x, y) # :nodoc:
      if x <= 85
        return 0 if y <= 85
        return 3 if y <= 170
        return 6
      elsif x <= 170
        return 1 if y <= 85
        return 4 if y <= 170
        return 7
      else
        return 2 if y <= 85
        return 5 if y <= 170
        return 8
      end
    end

    def bloc_pos(id) # :nodoc:
      case_width  = @w / 3
      case_height = @h / 3

      if id % 3 == 0
        case_x = self.x
      elsif id % 3 == 1
        case_x = self.x + case_width
      else
        case_x = self.x + (case_width * 2)
      end
      
      if id < 3
        case_y = self.y
      elsif id < 6
        case_y = self.y + case_height
      else
        case_y = self.y + (case_height * 2)
      end

      return Point.new(case_x, case_y)
    end

    def pressed_str(id) # :nodoc:
      str_id = 0 if Pad.pressed? @keys[:bottom]
      str_id = 1 if Pad.pressed? @keys[:up]
      str_id = 2 if Pad.pressed? @keys[:left]
      str_id = 3 if Pad.pressed? @keys[:right]

      return nil unless str_id
      return @pages[@page_id][id][str_id]
    end

    # Draws the keyboard, and eventually update its string.
    def draw
      @page_id += 1 if Pad.pressed? @keys[:next_page] and @pages[@page_id + 1]
      @page_id -= 1 if Pad.pressed? @keys[:prev_page] and @pages[@page_id - 1]

      focused_id = selected_id(Pad.stickX + 127, Pad.stickY + 127)
      str = pressed_str(focused_id)
      if str == -1
        @str[-1] = "" if @str[-1]
      elsif str
        @str += str
      end

      for id in 0..8
        point  = bloc_pos(id)
        width  = @w / 3
        height = @h / 3

        symbol = id == focused_id ? :focus : :normal
        Joyau.setTextColor(@txt_colors[symbol])
        
        rect = DrawableRect.new(point.x, point.y, 
                                point.x + width, point.y + height)

        # 1. Draw the backgroud
        if @bg_colors[symbol].is_a? Sprite
          @bg_colors[symbol].pos = point
          @bg_colors[symbol].setTile(0, 0, width, height)
          @bg_colors[symbol].draw
        else
          rect.color = @bg_colors[symbol]
          rect.draw
        end
        
        # 2. Draw the border
        rect.color = @border_col
        rect.filled = false
        rect.draw

        # 3. Draw the strings
        str = @pages[@page_id][id][0]
        str = "Del" if str == -1
        str = "Spc" if str == ' '

        str_x = point.x + (width / 2) - (Joyau.getLength(str) / 2)
        str_y = point.y + height - 9

        Joyau.drawText(str_x, str_y, str)
        
        str = @pages[@page_id][id][1]
        str = "Del" if str == -1
        str = "Spc" if str == ' '

        str_x = point.x + (width / 2) - (Joyau.getLength(str) / 2)
        str_y = point.y + 1

        Joyau.drawText(str_x, str_y, str)

        str = @pages[@page_id][id][2]
        str = "Del" if str == -1
        str = "Spc" if str == ' '

        str_x = point.x + 1
        str_y = point.y + (height / 2) - 5

        Joyau.drawText(str_x, str_y, str)

        str = @pages[@page_id][id][3]
        str = "Del" if str == -1
        str = "Spc" if str == ' '

        str_x = point.x + width - Joyau.getLength(str)
        str_y = point.y + (height / 2) - 5

        Joyau.drawText(str_x, str_y, str)
      end
    end

    # Keyboard's geometry
    attr_accessor :w, :h
    
    # A hash whose keys :normal and :focus are checked. The color
    # You give there is set for the text which is either focused or not.
    attr_accessor :txt_colors
    
    # Like txt_colors, but for the background. Notice that a Sprite can be given
    # too here.
    attr_accessor :bg_colors

    # The border's color.
    attr_accessor :border_col

    # A Hash where you can set the keys at which the keyboards should react.
    # :next_page, :prev_page, :bottom, :up, :left, and :right are the valid
    # keys.
    attr_accessor :keys

    # Three dimension array containing all the keys which are pressed
    # by the user. Use it so : level[page_id][case_id][char_id].
    attr_accessor :pages

    # Contains the message entered by the user.
    attr_accessor :str

    # Retuns the selected page's id.
    attr_reader :page_id
  end
end
