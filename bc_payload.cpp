//#include <iostream>;
///*
// *  例：20 53 01 28  计算结果为8，位置为d9-dc，js算法如下
// *  var answer = 0x20; answer |= 0x53 << 8; answer |= 0x1 << 16; answer |= 0x28 << 24;
// *  var bytes = (answer & 3) + 1; mask = parseInt(0xffffffff, 16); mask >>= 32 - (bytes << 3); mask = Math.abs(mask);
// *  answer &= mask; answer >>= 2; 
// */
//inline int GetInt() {
//    // This way of decoding variable-length encoded integers does not
//    // suffer from branch mispredictions.
//    DCHECK(position_ + 3 < length_);
//    uint32_t answer = data_[position_];
//    answer |= data_[position_ + 1] << 8;
//    answer |= data_[position_ + 2] << 16;
//    answer |= data_[position_ + 3] << 24;
//    int bytes = (answer & 3) + 1;
//    Advance(bytes);
//    uint32_t mask = 0xffffffffu;
//    mask >>= 32 - (bytes << 3);
//    answer &= mask;
//    answer >>= 2;
//    return answer;
//}
//
//
//int Deserializer<IsolateT>::ReadSingleBytecodeData(byte data,
//    SlotAccessor slot_accessor) {
//
//
//
//
//    Deserializer<IsolateT>::Deserializer(IsolateT * isolate,
//        base::Vector<const byte> payload,
//        uint32_t magic_number,
//        bool deserializing_user_code,
//        bool can_rehash)
//        : isolate_(isolate),
//        source_(payload),
//        magic_number_(magic_number),
//        deserializing_user_code_(deserializing_user_code),
//        can_rehash_(can_rehash) {
//        DCHECK_NOT_NULL(isolate);
//        isolate->RegisterDeserializerStarted();
//
//
//
//
//        template <typename IsolateT>
//        void Deserializer<IsolateT>::ReadData(Handle<HeapObject> object,
//            int start_slot_index,
//            int end_slot_index) {
//            int current = start_slot_index;
//            while (current < end_slot_index) {
//                byte data = source_.Get();
//                current += ReadSingleBytecodeData(
//                    data, SlotAccessorForHeapObject::ForSlotIndex(object, current));
//            }
//            CHECK_EQ(current, end_slot_index);
//        }