#
# old.rb - backward compatibility for Joyau
# Copyright 2009 Verhetsel Kilian
#
# This program can be distributed under the terms of the GNU GPL.
# See the file COPYING.
#

warn "Warning:#{caller[0].sub(/:in `.*'\z/, '')}: including Joyau in the global
namespace."

# We won't include the full namespace. We'll only insert the classes
# which were defined before the creation of this namespace.

Vector3f = Joyau::Vector3f
AudioObject = Joyau::Vector3f
Sound = Joyau::Sound
Stream = Joyau::Stream
Listener = Joyau::Listener
Circle = Joyau::Circle
CircleMenu = Joyau::CircleMenu
Point = Joyau::Point
Rect = Joyau::Rect
Drawable = Joyau::Drawable
DrawableRect = Joyau::DrawableRect
DrawableText = Joyau::DrawableText
Intrafont = Joyau::Intrafont
IntraText = Joyau::IntraText
CollisionType = Joyau::CollisionType
Tile = Joyau::Tile
GameMap = Joyau::GameMap
Umd = Joyau::Umd
Pad = Joyau::Pad
Cursor = Joyau::Cursor
Line = Joyau::Line
Message = Joyau::Message
MsgConfig = Joyau::MsgConfig
VerticalMsgSelecter = Joyau::VerticalMsgSelecter
HorizontalMsgSelecter = Joyau::HorizontalMsgSelecter
MultiHorizontalMsgSelecter = Joyau::MultiHorizontalMsgSelecter
MultiVerticalMsgSelecter = Joyau::MultiVerticalMsgSelecter
Particles = Joyau::Particles
Scrolling = Joyau::Scrolling
Shape = Joyau::Shape
FillableShape = Joyau::Shape
Sprite = Joyau::Sprite
Timer = Joyau::Timer
Triangle = Joyau::Triangle
Usb = Joyau::Usb

def initAudio
  Joyau::initAudio
end

def stopAudio
  Joyau::stopAudio
end

def swizzle
  Joyau::swizzle
end

def mayPlay
  Joyau::mayPlay
end

def startDraw
  Joyau::startDraw
end

def endDraw
  Joyau::endDraw
end

def sync
  Joyau::sync
end

def frameskip(min, max)
  Joyau::frameskip(min, max)
end

def color(r, g, b, a = 255)
  Joyau::color(r, g, b, a)
end

def drawLine(x1, y1, x2, y2, col)
  Joyau::drawLine(x1, y1, x2, y2, col)
end

def drawRect(x1, y1, x2, y2, col)
  Joyau::drawLine(x1, y1, x2, y2, col)
end

def drawFillRect(x1, y1, x2, y2, col)
  Joyau::drawFillRect(x1, y1, x2, y2, col)
end

def drawTriangle(x1, y1, x2, y2, x3, y3, col1, col2, col3)
  Joyau::drawTriangle(x1, y1, x2, y2, x3, y3, col1, col2, col3)
end

def drawCircle(x, y, rad, col)
  Joyau::drawCircle(x, y, rad, col)
end

def drawFillCircle(x, y, rad, col)
  Joyau::drawFillCircle(x, y, rad, col)
end

def screenshot(file)
  Joyau::screenshot(file)
end

def fade
  Joyau::fade
end

def setFont(name)
  Joyau::setFont(name)
end

def getLength(str)
  Joyau::getLength(str)
end

def setTextBackground(col)
  Joyau::setTextBackground(col)
end

def setTextColor(col)
  Joyau::setTextColor(col)
end

def drawText(x, y, txt)
  Joyau::drawText(x, y, txt)
end

def drawScripted(x, y, txt)
  Joyau::drawScripted(x, y, txt)
end

def drawStirringText(x, y, txt)
  Joyau::drawStirringText(x, y, txt)
end

def clearScreen
  Joyau::clearScreen
end

def initGfx
  Joyau::initGfx
end

def initLib
  Joyau::initLib
end

def stopLib
  Joyau::stopLib
end

def stopGfx
  Joyau::stopGfx
end

def cd(dir)
  Joyau::cd(dir)
end

def timestamp
  Joyau::timestamp
end

def powerTime
  Joyau::powerTime
end

def powerPercent
  Joyau::powerPercent
end

def mkdir(dir)
  Joyau::mkdir(dir)
end

def rmdir(dir)
  Joyau::rmdir(dir)
end

def rm(file)
  Joyau::rm(file)
end

def mv(file)
  Joyau::mv(file)
end

def readKeys
  Joyau::readKeys
end

def gets
  Joyau::gets
end

def clearImages
  Joyau::clearImages
end

def clearFonts
  Joyau::clearFonts
end

def clearSounds
  Joyau::clearSounds
end

def debug(str)
  Joyau::debug(str)
end

def puts(*args)
  args.each { |i| Joyau::puts(i) }
end

def exitGame
  Joyau::exitGame
end

$directions = { 
  "LEFT" => Joyau::Sprite::LEFT,
  "RIGHT" => Joyau::Sprite::RIGHT,
  "DOWN" => Joyau::Sprite::DOWN,
  "UP" => Joyau::Sprite::UP,
  "UP_LEFT" => Joyau::Sprite::UP_LEFT,
  "UP_RIGHT" => Joyau::Sprite::UP_RIGHT,
  "DOWN_LEFT" => Joyau::Sprite::DOWN_LEFT,
  "DOWN_RIGHT" => Joyau::Sprite::DOWN_RIGHT
}
