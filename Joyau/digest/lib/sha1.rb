# just for compatibility; requiring "sha1" is obsoleted
#
# $RoughId: sha1.rb,v 1.4 2001/07/13 15:38:27 knu Exp $
# $Id: sha1.rb 12008 2007-03-06 10:12:12Z knu $

require 'digest'

if defined? Joyau and not defined? Digest::SHA1
  Joyau.init_internal("sha1")
end

class SHA1 < Digest::SHA1
  class << self
    alias orig_new new
    def new(str = nil)
      if str
        orig_new.update(str)
      else
        orig_new
      end
    end

    def sha1(*args)
      new(*args)
    end
  end
end
