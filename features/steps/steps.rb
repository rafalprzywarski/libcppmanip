require 'rspec'
require './buildpath'

SOURCE_FILE="source.cpp"

def rangeFromPhrases startPhrase, endPhrase, source
  startIndex = $SOURCE.index(startPhrase)
  endIndex = $SOURCE.index(endPhrase)
  startIndex.should_not be_nil, "\"#{startPhrase}\" not found in the source code"
  endIndex.should_not be_nil, "\"#{endPhrase}\" not found in the source code"
  startIndex.should be <= endIndex, "\"#{startPhrase}\" must be before or equal to \"#{endPhrase}\""
  endIndex += endPhrase.length
  return startIndex, endIndex
end

After do
  File.delete(SOURCE_FILE)
end

Given /^source code:$/ do |source|
  $SOURCE = source
end

When /^I run method extraction from "(.*?)" to "(.*?)" with name "(.*?)"$/ do |startPhrase, endPhrase, methodName|
  startOffset, endOffset = rangeFromPhrases startPhrase, endPhrase, $SOURCE
  File.open(SOURCE_FILE, "w") { |f| f.write $SOURCE }
  $cppmanip_output = %x(#{BUILD_DIRECTORY}/runner/cppmaniprunner_extract_method #{SOURCE_FILE} #{methodName} #{startOffset} #{endOffset})
  $cppmanip_exit_code = $?
end

When /^I run method extraction for "(.*?)" with name "(.*?)"$/ do |phrase, methodName|
  step "I run method extraction from \"#{phrase}\" to \"#{phrase}\" with name \"#{methodName}\""
end

def shouldNotFail
  $cppmanip_exit_code.should eq(0), "cppmanip failed with error code #{$cppmanip_exit_code}: #{$cppmanip_output}"
end

Then /^final source code should be:$/ do |expectedSource|
  shouldNotFail
  File.read(SOURCE_FILE).should eq(expectedSource)
end

Then /^final source code should contain:$/ do |expectedSource|
  shouldNotFail
  File.read(SOURCE_FILE).should include(expectedSource)
end

Then /^it should fail with a message "(.*?)"$/ do |expectedMessage|
  $cppmanip_exit_code.should_not eq(0), "cppmanip should have failed with message: #{expectedMessage}"
  $cppmanip_output.should include(expectedMessage)
end
