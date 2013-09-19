require 'rexml/document'

def loadError filename
  doc = File.open(filename) { |f| REXML::Document.new f }
  doc.elements["boost_serialization/error"].text
end
