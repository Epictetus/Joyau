# This fixes DRb, so that it could be used on your PSP.
# In fact, it only redefines getservername.

require 'drb/drb'

module DRb
  module DRbProtocol
    def self.getservername
      'localhost'
    end
  end
end
