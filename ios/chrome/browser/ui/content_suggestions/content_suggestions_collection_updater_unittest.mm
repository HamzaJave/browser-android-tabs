// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ios/chrome/browser/ui/content_suggestions/content_suggestions_collection_updater.h"

#import "ios/chrome/browser/ui/collection_view/collection_view_model.h"
#import "ios/chrome/browser/ui/content_suggestions/content_suggestion.h"
#import "ios/chrome/browser/ui/content_suggestions/content_suggestions_view_controller.h"
#import "ios/chrome/browser/ui/content_suggestions/identifier/content_suggestion_identifier.h"
#import "ios/chrome/browser/ui/content_suggestions/identifier/content_suggestions_section_information.h"
#include "testing/platform_test.h"
#import "third_party/ocmock/OCMock/OCMock.h"
#import "third_party/ocmock/gtest_support.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace {

TEST(ContentSuggestionsCollectionUpdaterTest, addEmptyItemToEmptySection) {
  // Setup.
  ContentSuggestionsCollectionUpdater* updater =
      [[ContentSuggestionsCollectionUpdater alloc] initWithDataSource:nil];
  CollectionViewModel* model = [[CollectionViewModel alloc] init];
  id mockCollection = OCMClassMock([ContentSuggestionsViewController class]);
  OCMStub([mockCollection collectionViewModel]).andReturn(model);
  updater.collectionViewController = mockCollection;

  ContentSuggestion* suggestion = [[ContentSuggestion alloc] init];
  suggestion.suggestionIdentifier = [[ContentSuggestionIdentifier alloc] init];
  suggestion.suggestionIdentifier.sectionInfo =
      [[ContentSuggestionsSectionInformation alloc]
          initWithSectionID:ContentSuggestionsSectionArticles];
  suggestion.suggestionIdentifier.sectionInfo.showIfEmpty = YES;
  [updater addSectionsForSuggestionsToModel:@[ suggestion ]];
  ASSERT_EQ(0, [model numberOfItemsInSection:0]);

  // Action.
  NSIndexPath* addedItem = [updater addEmptyItemForSection:0];

  // Test.
  EXPECT_TRUE([[NSIndexPath indexPathForItem:0 inSection:0] isEqual:addedItem]);
  ASSERT_EQ(1, [model numberOfItemsInSection:0]);
}

TEST(ContentSuggestionsCollectionUpdaterTest, addEmptyItemToSection) {
  // Setup.
  ContentSuggestionsCollectionUpdater* updater =
      [[ContentSuggestionsCollectionUpdater alloc] initWithDataSource:nil];
  CollectionViewModel* model = [[CollectionViewModel alloc] init];
  id mockCollection = OCMClassMock([ContentSuggestionsViewController class]);
  OCMStub([mockCollection collectionViewModel]).andReturn(model);
  updater.collectionViewController = mockCollection;

  ContentSuggestion* suggestion = [[ContentSuggestion alloc] init];
  suggestion.suggestionIdentifier = [[ContentSuggestionIdentifier alloc] init];
  suggestion.suggestionIdentifier.sectionInfo =
      [[ContentSuggestionsSectionInformation alloc]
          initWithSectionID:ContentSuggestionsSectionArticles];
  suggestion.suggestionIdentifier.sectionInfo.showIfEmpty = YES;
  [updater addSectionsForSuggestionsToModel:@[ suggestion ]];
  [updater addSuggestionsToModel:@[ suggestion ]];
  ASSERT_EQ(1, [model numberOfItemsInSection:0]);

  // Action.
  NSIndexPath* addedItem = [updater addEmptyItemForSection:0];

  // Test.
  EXPECT_TRUE([[NSIndexPath indexPathForItem:1 inSection:0] isEqual:addedItem]);
  ASSERT_EQ(2, [model numberOfItemsInSection:0]);
}

}  // namespace
