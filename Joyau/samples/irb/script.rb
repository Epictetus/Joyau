require 'irb'

class JoyauInputMethod < IRB::InputMethod
  def initialize
    super
    @line_no = 0
    @line = []
  end

  def gets
    begin
      Joyau::Pad.update
    end until Joyau::Pad.held? Joyau::Pad::CROSS

    @line[@line_no += 1] = Joyau.gets + "\n"
    Joyau::puts(@prompt.to_s + @line.to_s)

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
end

class JoyauOutputMethod < IRB::OutputMethod
  def initialize
    super
    @olds = []
  end

  def puts(*opts)
    @olds.remove_at(0) if @olds.size >= 45
    opts.each { |i| @olds << i.to_s }
    y = 0
    
    Joyau.startDraw
    Joyau.clearScreen
    @olds.each { |i|
      Joyau.drawScripted(0, y, i + "\n")
      y += 10
    }
    Joyau.endDraw

    Joyau.sync
  end

  def print(*opts)
    @olds.remove_at(0) if @olds.size >= 45
    opts.each { |i| @olds << i.to_s }
    y = 0
    
    Joyau.startDraw
    Joyau.clearScreen
    @olds.each { |i|
      Joyau.drawScripted(0, y, i)
      y += 10
    }
    Joyau.endDraw

    Joyau.sync
  end
end

module IRB
  def IRB.start(ap_path = nil)
    $0 = File::basename(ap_path, ".rb") if ap_path
    
    IRB.setup(ap_path)
    
    irb = Irb.new(nil, JoyauInputMethod.new, JoyauOutputMethod.new)
    
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
