/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "Librarian.h"
#include "MidiController.h"
#include "Logger.h"

#include "Patch.h"
#include "Synth.h"
#include "SynthHolder.h"

#include "PatchButtonPanel.h"
#include "SplitteredComponent.h"
#include "CategoryButtons.h"
#include "CurrentPatchDisplay.h"
#include "CollapsibleContainer.h"
#include "PatchListTree.h"
#include "RecycleBin.h"

#include "PatchDatabase.h"
#include "PatchHolder.h"
#include "AutomaticCategory.h"

#include "ImportFromSynthDialog.h"
#include "SynthBankPanel.h"

#include <map>

class PatchDiff;
class PatchSearchComponent;

class PatchView : public Component,
	public DragAndDropContainer,
	private ChangeListener
{
public:
	PatchView(midikraft::PatchDatabase &database, std::vector<midikraft::SynthHolder> const &synths);
	virtual ~PatchView() override;

	void resized() override;

	// React on synth or patch changed
	virtual void changeListenerCallback(ChangeBroadcaster* source) override;

	void retrieveFirstPageFromDatabase();
	std::shared_ptr<midikraft::PatchList> retrieveListFromDatabase(midikraft::ListInfo const& info);
	void loadSynthBankFromDatabase(std::shared_ptr<midikraft::Synth> synth, MidiBankNumber bank, std::string const& bankId);
	void retrieveBankFromSynth(std::shared_ptr<midikraft::Synth> synth, MidiBankNumber bank, std::function<void()> finishedHandler);
	void sendBankToSynth(std::shared_ptr<midikraft::SynthBank> bankToSend, bool ignoreDirty, std::function<void()> finishedHandler);
	void selectPatch(midikraft::PatchHolder& patch, bool alsoSendToSynth);

	// Macro controls triggered by the MidiKeyboard
	void hideCurrentPatch();
	void favoriteCurrentPatch();
	void selectPreviousPatch();
	void selectNextPatch();
	void retrieveEditBuffer();

	// Protected functions that are potentially dangerous and are only called via the main menu
	void retrievePatches();
	void bulkRenamePatches();
	void receiveManualDump();
	void deletePatches();
	void reindexPatches();
	void loadPatches();
	void exportPatches();
	void createPatchInterchangeFile();
	void showPatchDiffDialog();

	// Additional functions for the auto thumbnailer
	int totalNumberOfPatches();
	void selectFirstPatch();

	// Hand through from PatchSearch
	midikraft::PatchFilter currentFilter();

	// Special functions
	void bulkImportPIP(File directory);

private:
	friend class PatchSearchComponent;

	std::vector<CategoryButtons::Category> predefinedCategories();

	void loadPage(int skip, int limit, midikraft::PatchFilter const& filter, std::function<void(std::vector<midikraft::PatchHolder>)> callback);

	// New for bank management
	midikraft::PatchFilter bankFilter(std::shared_ptr<midikraft::Synth> synth, std::string const& listID);

	std::vector<midikraft::PatchHolder> autoCategorize(std::vector<midikraft::PatchHolder> const &patches);

	void updateLastPath();

	void mergeNewPatches(std::vector<midikraft::PatchHolder> patchesLoaded);
	
	void saveCurrentPatchCategories();
	void setSynthBankFilter(std::shared_ptr<midikraft::Synth> synth, MidiBankNumber bank);
	void setUserBankFilter(std::shared_ptr<midikraft::Synth> synth, std::string const& listId);
	void setImportListFilter(String filter);
	void setUserListFilter(String filter);
	void deleteSomething(nlohmann::json const &infos);

	void showBank();

	PatchListTree patchListTree_;
	std::string sourceFilterID_; // This is the old "import" combo box in new
	std::string listFilterID_;
	std::unique_ptr<SplitteredComponent> splitters_;
	TabbedComponent rightSideTab_;

	RecycleBin recycleBin_;

	Label patchLabel_;
	std::unique_ptr<PatchSearchComponent> patchSearch_;
	std::unique_ptr<PatchButtonPanel> patchButtons_;
	std::unique_ptr<CurrentPatchDisplay> currentPatchDisplay_;
	std::unique_ptr<SynthBankPanel> synthBank_;
	std::unique_ptr<ImportFromSynthDialog> importDialog_;
	std::unique_ptr<PatchDiff> diffDialog_;

	midikraft::Librarian librarian_;

	std::vector<midikraft::SynthHolder> synths_;
	int currentLayer_;

	midikraft::PatchHolder compareTarget_;

	midikraft::PatchDatabase &database_;
	
	std::string lastPathForPIF_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchView)
};

