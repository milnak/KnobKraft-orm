/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "Synth.h"
#include "MidiBankNumber.h"
#include "ProgressHandler.h"

class ImportFromSynthDialog : public Component
{
public:
	typedef std::function<void(std::vector<MidiBankNumber> bankNo)> TSuccessHandler;

	ImportFromSynthDialog(std::shared_ptr<midikraft::Synth> synth, TSuccessHandler onOk);
	virtual ~ImportFromSynthDialog() override = default;

	void resized() override;

private:
	TSuccessHandler onOk_;
	MultiChoicePropertyComponent  *banks_;
	PropertyPanel propertyPanel_;
	TextButton ok_, all_, cancel_;

	Value bankValue_;
	int numBanks_; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImportFromSynthDialog)
};

