require 'rspec'
require './buildpath'
require './features/steps/replacements'
require './features/steps/errors'

SOURCE_FILE="source.cpp"
REPLACEMENTS_FILE="replacements.xml"
ERROR_FILE="errors.xml"

Transform /^(-?\d+)$/ do |number|
  number.to_i
end

def rangeFromPhrases startPhrase, endPhrase, source
  startIndex = source.index(startPhrase)
  endIndex = source.index(endPhrase)
  startIndex.should_not be_nil, "\"#{startPhrase}\" not found in the source code"
  endIndex.should_not be_nil, "\"#{endPhrase}\" not found in the source code"
  startIndex.should be <= endIndex, "\"#{startPhrase}\" must be before or equal to \"#{endPhrase}\""
  endIndex += endPhrase.length
  return startIndex, endIndex
end

After do
  [ SOURCE_FILE, REPLACEMENTS_FILE, ERROR_FILE ].each do |f|
    File.delete(f) if File.file?(f)
  end
end

Given /^source code:$/ do |source|
  $SOURCE = source
end

When /^I run method extraction from "(.*?)" to "(.*?)" with name "(.*?)"$/ do |startPhrase, endPhrase, methodName|
  startOffset, endOffset = rangeFromPhrases startPhrase, endPhrase, $SOURCE
  File.open(SOURCE_FILE, "w") { |f| f.write $SOURCE }
  $cppmanip_output = %x(#{BUILD_DIRECTORY}/runner/cppmaniprunner_extract_method #{SOURCE_FILE} #{methodName} #{startOffset} #{endOffset} 2>&1)
  $cppmanip_exit_code = $?
end

When /^I run method extraction for "(.*?)" with name "(.*?)"$/ do |phrase, methodName|
  step "I run method extraction from \"#{phrase}\" to \"#{phrase}\" with name \"#{methodName}\""
end

When /^I run two method extractions for "(.*?)" with name "(.*?)" and for "(.*?)" with name "(.*?)"$/ do |phrase1, methodName1, phrase2, methodName2|
  startOffset1, endOffset1 = rangeFromPhrases phrase1, phrase1, $SOURCE
  startOffset2, endOffset2 = rangeFromPhrases phrase2, phrase2, $SOURCE
  File.open(SOURCE_FILE, "w") { |f| f.write $SOURCE }
  $cppmanip_output = %x(#{BUILD_DIRECTORY}/runner/cppmaniprunner_extract_method #{SOURCE_FILE} #{methodName1} #{startOffset1} #{endOffset1} #{methodName2} #{startOffset2} #{endOffset2} 2>&1)
  $cppmanip_exit_code = $?
end

When /^I run function extraction for "(.*?)" with name "(.*?)"$/ do |phrase, functionName|
  step "I run function extraction from \"#{phrase}\" to \"#{phrase}\" with name \"#{functionName}\""
end

When /^I run function extraction from "(.*?)" to "(.*?)" with name "(.*?)"$/ do |startPhrase, endPhrase, functionName|
  startOffset, endOffset = rangeFromPhrases startPhrase, endPhrase, $SOURCE
  File.open(SOURCE_FILE, "w") { |f| f.write $SOURCE }
  $cppmanip_output = %x(#{BUILD_DIRECTORY}/runner/cppmaniprunner_extract_function #{SOURCE_FILE} #{functionName} #{startOffset} #{endOffset} 2>&1)
  $?.should eq(0), "cppmanip failed with error \'#{$?}\': #{$cppmanip_output}"
  @replacements = loadReplacementsFromXml(REPLACEMENTS_FILE) if File.file?(REPLACEMENTS_FILE)
  @error = loadError(ERROR_FILE) if @replacements.nil?
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
  @error.should eq(expectedMessage), "cppmanip should have failed with message: #{expectedMessage}"
end

Then /^original code should not change$/ do
  File.read(SOURCE_FILE).should eq($SOURCE)
end

Given /^libcppmanip repository$/ do
    #we are in it
end

When /^I install it$/ do
  pending # express the regexp above with the code you wish you had
end

Then /^another project using it should compile and link$/ do
  pending # express the regexp above with the code you wish you had
end

Then /^there should be (\d+) changes$/ do |changeCount|
  @replacements.count.should eq(changeCount)
end

Then /^there should be an insertion:$/ do |insertionText|
  @replacements.index { |r| r.isInsertion && r.text == insertionText }.should_not be_nil, "insertion with \'#{insertionText}\' not found in #{@replacements}"
end

Then /^there should be an insertion before "(.*?)":$/ do |before, insertionText|
  @replacements.index { |r| r.isInsertionBefore(before, $SOURCE) && r.text == insertionText }.should_not be_nil
end

Then /^there should be a replacement with "(.*?)"$/ do |replacementText|
  @replacements.index { |r| r.text == replacementText }.should_not be_nil, "replacement with \'#{replacementText}\' not found in #{@replacements}"
end

Then /^there should be a replacement from "(.*?)" to "(.*?)" with "(.*?)"$/ do |from, to, replacementText|
  @replacements.index { |r|
    r.text == replacementText && r.isFrom(from, $SOURCE) && r.isTo(to, $SOURCE)
  }.should_not be_nil, "replacement with \'#{replacementText}\' not found in #{@replacements}"
end

Then /^there should be a replacement for "(.*?)" with "(.*?)"$/ do |phrase, replacementText|
  step "there should be a replacement from \"#{phrase}\" to \"#{phrase}\" with \"#{replacementText}\""
end
