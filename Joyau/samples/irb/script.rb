require 'irb'
require 'joyau/danzeff'

class JoyauInputMethod < IRB::InputMethod
  def initialize
    super
    
    @line_no = 0
    @line = []

    @keyboard = Joyau::Danzeff.new
    @keyboard.w, @keyboard.h = 480, 100
    @keyboard.pos = Joyau::Point.new(0, 0)
    @keyboard.bg_colors = {
      :normal  => Joyau::Sprite.new("bg1.png"),
      :focus   => Joyau::Sprite.new("bg2.png")
    }
    
    @output = JoyauOutputMethod.new
  end

  def gets
    begin
      Joyau::Pad.update

      Joyau.startDraw
      Joyau.clearScreen
      @keyboard.draw
      @output.drawOld(110)
      Joyau.drawScripted(0, 100, "==> " + @keyboard.str)
      Joyau.endDraw

      Joyau.sync
    end until Joyau::Pad.pressed? Joyau::Pad::START

    @line[@line_no += 1] = @keyboard.str + "\n"
    @output.puts(@line[@line_no])
    @keyboard.str = ""
    return @line[@line_no]
  end

  def eof?
    false
  end

  def readable_atfer_eof?
    true
  end

  def line(line_no)
    @line[line_no]
  end

  attr_reader :output
end

class JoyauOutputMethod < IRB::OutputMethod
  def initialize
    super
    @olds = []
  end

  def puts(*opts)
    @olds.delete_at(0) if @olds.size >= 16
    opts.each { |i| @olds << i.to_s }
  end

  def print(*opts)
    @olds.delete_at(0) if @olds.size >= 16
    opts.each { |i| @olds << i.to_s }
  end

  def drawOld(y)
    @olds.each { |i|
      Joyau.drawScripted(0, y, i)
      y += 10
    }
  end
end

module IRB
  def IRB.start(ap_path = nil)
    $0 = File::basename(ap_path, ".rb") if ap_path
    
    IRB.setup(ap_path)
    
    input = JoyauInputMethod.new
    output = input.output
    irb = Irb.new(nil, input, output)
    
    @CONF[:IRB_RC].call(irb.context) if @CONF[:IRB_RC]
    @CONF[:MAIN_CONTEXT] = irb.context
    
    trap("SIGINT") do
      irb.signal_handle
    end
  
    catch(:IRB_EXIT) do
      irb.eval_input
    end
  end
end

Joyau.initLib
Joyau.initGfx

IRB.start

Joyau.stopGfx
Joyau.stopLib
