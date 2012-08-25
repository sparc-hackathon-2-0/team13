/*
 *  EAN8Reader.cpp
 *  ZXing
 *
 *  Copyright 2010 ZXing authors All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "EAN8Reader.h"
#include <zxing/ReaderException.h>

namespace zxing {
  namespace oned {

    EAN8Reader::EAN8Reader(){ }

    int EAN8Reader::decodeMiddle(Ref<BitArray> row, int startRange[], int startRangeLen,
        std::string& resultString){
      const int countersLen = 4;
      int counters[countersLen] = { 0, 0, 0, 0 };

      int end = row->getSize();
      int rowOffset = startRange[1];

      for (int x = 0; x < 4 && rowOffset < end; x++) {
        int bestMatch = decodeDigit(row, counters, countersLen, rowOffset,
            UPC_EAN_PATTERNS_L_PATTERNS);
        if (bestMatch < 0) {
          return -1;
        }
        resultString.append(1, (char) ('0' + bestMatch));
        for (int i = 0; i < countersLen; i++) {
          rowOffset += counters[i];
        }
      }

      int* middleRange = findGuardPattern(row, rowOffset, true, (int*)getMIDDLE_PATTERN(),
            getMIDDLE_PATTERN_LEN());
      if (middleRange != NULL) {
        rowOffset = middleRange[1];
        for (int x = 0; x < 4 && rowOffset < end; x++) {
          int bestMatch = decodeDigit(row, counters, countersLen, rowOffset,
              UPC_EAN_PATTERNS_L_PATTERNS);
          if (bestMatch < 0) {
            delete [] middleRange;
            return -1;
          }
          resultString.append(1, (char) ('0' + bestMatch));
          for (int i = 0; i < countersLen; i++) {
            rowOffset += counters[i];
          }
        }

        delete [] middleRange;
        return rowOffset;
      }
      return -1;
    }

    BarcodeFormat EAN8Reader::getBarcodeFormat(){
      return BarcodeFormat_EAN_8;
    }
  }
}