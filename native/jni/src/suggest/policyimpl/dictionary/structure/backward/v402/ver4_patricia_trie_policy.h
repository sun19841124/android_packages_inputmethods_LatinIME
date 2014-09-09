/*
 * Copyright (C) 2013, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * !!!!! DO NOT CHANGE THE LOGIC IN THIS FILE !!!!!
 * Do not edit this file other than updating policy's interface.
 *
 * This file was generated from
 *   suggest/policyimpl/dictionary/structure/v4/ver4_patricia_trie_policy.h
 */

#ifndef LATINIME_BACKWARD_V402_VER4_PATRICIA_TRIE_POLICY_H
#define LATINIME_BACKWARD_V402_VER4_PATRICIA_TRIE_POLICY_H

#include <vector>

#include "defines.h"
#include "suggest/core/dictionary/binary_dictionary_bigrams_iterator.h"
#include "suggest/core/dictionary/binary_dictionary_shortcut_iterator.h"
#include "suggest/core/policy/dictionary_structure_with_buffer_policy.h"
#include "suggest/policyimpl/dictionary/header/header_policy.h"
#include "suggest/policyimpl/dictionary/structure/pt_common/dynamic_pt_updating_helper.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/bigram/ver4_bigram_list_policy.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/shortcut/ver4_shortcut_list_policy.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/ver4_dict_buffers.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/ver4_patricia_trie_node_reader.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/ver4_patricia_trie_node_writer.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/ver4_patricia_trie_writing_helper.h"
#include "suggest/policyimpl/dictionary/structure/backward/v402/ver4_pt_node_array_reader.h"
#include "suggest/policyimpl/dictionary/utils/buffer_with_extendable_buffer.h"
#include "utils/int_array_view.h"

namespace latinime {
namespace backward {
namespace v402 {

} // namespace v402
} // namespace backward
class DicNode;
namespace backward {
namespace v402 {
} // namespace v402
} // namespace backward
class DicNodeVector;
namespace backward {
namespace v402 {

// Word id = Position of a PtNode that represents the word.
class Ver4PatriciaTriePolicy : public DictionaryStructureWithBufferPolicy {
 public:
    Ver4PatriciaTriePolicy(Ver4DictBuffers::Ver4DictBuffersPtr buffers)
            : mBuffers(std::move(buffers)), mHeaderPolicy(mBuffers->getHeaderPolicy()),
              mDictBuffer(mBuffers->getWritableTrieBuffer()),
              mBigramPolicy(mBuffers->getMutableBigramDictContent(),
                      mBuffers->getTerminalPositionLookupTable(), mHeaderPolicy),
              mShortcutPolicy(mBuffers->getMutableShortcutDictContent(),
                      mBuffers->getTerminalPositionLookupTable()),
              mNodeReader(mDictBuffer, mBuffers->getProbabilityDictContent(), mHeaderPolicy),
              mPtNodeArrayReader(mDictBuffer),
              mNodeWriter(mDictBuffer, mBuffers.get(), mHeaderPolicy, &mNodeReader,
                      &mPtNodeArrayReader, &mBigramPolicy, &mShortcutPolicy),
              mUpdatingHelper(mDictBuffer, &mNodeReader, &mNodeWriter),
              mWritingHelper(mBuffers.get()),
              mUnigramCount(mHeaderPolicy->getUnigramCount()),
              mBigramCount(mHeaderPolicy->getBigramCount()),
              mTerminalPtNodePositionsForIteratingWords(), mIsCorrupted(false) {};

    virtual int getRootPosition() const {
        return 0;
    }

    void createAndGetAllChildDicNodes(const DicNode *const dicNode,
            DicNodeVector *const childDicNodes) const;

    int getCodePointsAndProbabilityAndReturnCodePointCount(
            const int wordId, const int maxCodePointCount, int *const outCodePoints,
            int *const outUnigramProbability) const;

    int getWordId(const CodePointArrayView wordCodePoints, const bool forceLowerCaseSearch) const;

    int getProbabilityOfWordInContext(const int *const prevWordIds, const int wordId,
            MultiBigramMap *const multiBigramMap) const;

    int getProbability(const int unigramProbability, const int bigramProbability) const;

    int getProbabilityOfWord(const int *const prevWordIds, const int wordId) const;

    void iterateNgramEntries(const int *const prevWordIds, NgramListener *const listener) const;

    BinaryDictionaryShortcutIterator getShortcutIterator(const int wordId) const;

    const DictionaryHeaderStructurePolicy *getHeaderStructurePolicy() const {
        return mHeaderPolicy;
    }

    bool addUnigramEntry(const CodePointArrayView wordCodePoints,
            const UnigramProperty *const unigramProperty);

    bool removeUnigramEntry(const CodePointArrayView wordCodePoints);

    bool addNgramEntry(const PrevWordsInfo *const prevWordsInfo,
            const BigramProperty *const bigramProperty);

    bool removeNgramEntry(const PrevWordsInfo *const prevWordsInfo,
            const CodePointArrayView wordCodePoints);

    bool flush(const char *const filePath);

    bool flushWithGC(const char *const filePath);

    bool needsToRunGC(const bool mindsBlockByGC) const;

    void getProperty(const char *const query, const int queryLength, char *const outResult,
            const int maxResultLength);

    const WordProperty getWordProperty(const CodePointArrayView wordCodePoints) const;

    int getNextWordAndNextToken(const int token, int *const outCodePoints,
            int *const outCodePointCount);

    bool isCorrupted() const {
        return mIsCorrupted;
    }

 private:
    DISALLOW_IMPLICIT_CONSTRUCTORS(Ver4PatriciaTriePolicy);

    static const char *const UNIGRAM_COUNT_QUERY;
    static const char *const BIGRAM_COUNT_QUERY;
    static const char *const MAX_UNIGRAM_COUNT_QUERY;
    static const char *const MAX_BIGRAM_COUNT_QUERY;
    // When the dictionary size is near the maximum size, we have to refuse dynamic operations to
    // prevent the dictionary from overflowing.
    static const int MARGIN_TO_REFUSE_DYNAMIC_OPERATIONS;
    static const int MIN_DICT_SIZE_TO_REFUSE_DYNAMIC_OPERATIONS;

    const Ver4DictBuffers::Ver4DictBuffersPtr mBuffers;
    const HeaderPolicy *const mHeaderPolicy;
    BufferWithExtendableBuffer *const mDictBuffer;
    Ver4BigramListPolicy mBigramPolicy;
    Ver4ShortcutListPolicy mShortcutPolicy;
    Ver4PatriciaTrieNodeReader mNodeReader;
    Ver4PtNodeArrayReader mPtNodeArrayReader;
    Ver4PatriciaTrieNodeWriter mNodeWriter;
    DynamicPtUpdatingHelper mUpdatingHelper;
    Ver4PatriciaTrieWritingHelper mWritingHelper;
    int mUnigramCount;
    int mBigramCount;
    std::vector<int> mTerminalPtNodePositionsForIteratingWords;
    mutable bool mIsCorrupted;

    int getBigramsPositionOfPtNode(const int ptNodePos) const;
    int getShortcutPositionOfPtNode(const int ptNodePos) const;
    int getWordIdFromTerminalPtNodePos(const int ptNodePos) const;
    int getTerminalPtNodePosFromWordId(const int wordId) const;
};
} // namespace v402
} // namespace backward
} // namespace latinime
#endif // LATINIME_BACKWARD_V402_VER4_PATRICIA_TRIE_POLICY_H
