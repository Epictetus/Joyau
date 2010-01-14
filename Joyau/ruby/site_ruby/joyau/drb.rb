# This fixes DRb, so that it could be used on your PSP.
# In fact, it only redefines getservername.

require 'drb/drb'

module DRb
  class DRbTCPSocket
    def self.getservername
      'localhost'
    end
  end
end
