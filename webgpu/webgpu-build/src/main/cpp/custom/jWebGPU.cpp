#include "jWebGPU.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef __ANDROID__
// Custom streambuf to redirect std::cout to Logcat
int LogcatStreamBuf::overflow(int c) {
    if (c == EOF) {
        return !EOF;
    }
    buffer += static_cast<char>(c);
    if (c == '\n') {
        sync();
    }
    return c;
}

int LogcatStreamBuf::sync() {
    if (!buffer.empty()) {
        if (buffer.back() == '\n') {
            buffer.pop_back();
        }
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", buffer.c_str());
        buffer.clear();
    }
    return 0;
}

// Singleton to manage Logcat redirection
void LogcatRedirector::initialize() {
    static LogcatRedirector instance; // Lazy initialization
    if (!instance.isInitialized) {
        instance.original_buffer = std::cout.rdbuf();
        std::cout.rdbuf(&instance.logcat_buffer);
        instance.isInitialized = true;
    }
}

LogcatRedirector::LogcatRedirector() : original_buffer(nullptr), isInitialized(false) {}

LogcatRedirector::~LogcatRedirector() {
    if (isInitialized) {
        std::cout.rdbuf(original_buffer);
    }
}
#endif // __ANDROID__

// Template instantiations
template void WGPUByteBuffer::putNumeric<float>(int index, float value);
template void WGPUByteBuffer::putNumeric<int16_t>(int index, int16_t value);
template float WGPUByteBuffer::getNumeric<float>(int index);
template int16_t WGPUByteBuffer::getNumeric<int16_t>(int index);

template class WebGPUObjectBase<WebGPUCommandBuffer, WGPUCommandBuffer>;
template class WebGPUObjectBase<WebGPUBuffer, WGPUBuffer>;
template class WebGPUObjectBase<WebGPUQueue, WGPUQueue>;
template class WebGPUObjectBase<WebGPUBindGroupLayout, WGPUBindGroupLayout>;
template class WebGPUObjectBase<WebGPUComputePipeline, WGPUComputePipeline>;
template class WebGPUObjectBase<WebGPUStringView, WGPUStringView>;
template class WebGPUObjectBase<WebGPUChainedStruct, WGPUChainedStruct*>;
template class WebGPUObjectBase<WebGPUBindGroup, WGPUBindGroup>;
template class WebGPUObjectBase<WebGPURenderBundle, WGPURenderBundle>;
template class WebGPUObjectBase<WebGPUShaderModule, WGPUShaderModule>;
template class WebGPUObjectBase<WebGPUPipelineLayout, WGPUPipelineLayout>;
template class WebGPUObjectBase<WebGPURenderPipeline, WGPURenderPipeline>;
template class WebGPUObjectBase<WebGPURenderPassEncoder, WGPURenderPassEncoder>;
template class WebGPUObjectBase<WebGPUComputePassEncoder, WGPUComputePassEncoder>;
template class WebGPUObjectBase<WebGPUQuerySet, WGPUQuerySet>;
template class WebGPUObjectBase<WebGPUTextureView, WGPUTextureView>;
template class WebGPUObjectBase<WebGPUTexture, WGPUTexture>;
template class WebGPUObjectBase<WebGPUCommandEncoder, WGPUCommandEncoder>;
template class WebGPUObjectBase<WebGPUDevice, WGPUDevice>;
template class WebGPUObjectBase<WebGPUAdapter, WGPUAdapter>;
template class WebGPUObjectBase<WebGPUSurface, WGPUSurface>;
template class WebGPUObjectBase<WebGPUInstance, WGPUInstance>;

// WGPUVectorFutureWaitInfo
WGPUVectorFutureWaitInfo WGPUVectorFutureWaitInfo::Obtain() {
    WGPUVectorFutureWaitInfo obj;
    return obj;
}

int WGPUVectorFutureWaitInfo::size() { return vector.size(); }

void WGPUVectorFutureWaitInfo::push_back(const WebGPUFutureWaitInfo& entry) { vector.push_back(entry); }

const WebGPUFutureWaitInfo* WGPUVectorFutureWaitInfo::data() { return vector.data(); }


// WGPUVectorBindGroupEntry
WGPUVectorBindGroupEntry WGPUVectorBindGroupEntry::Obtain() {
    WGPUVectorBindGroupEntry obj;
    return obj;
}

int WGPUVectorBindGroupEntry::size() { return vector.size(); }

void WGPUVectorBindGroupEntry::push_back(const WebGPUBindGroupEntry& entry) { vector.push_back(entry); }

const WebGPUBindGroupEntry* WGPUVectorBindGroupEntry::data() { return vector.data(); }

// WGPUVectorColorTargetState
WGPUVectorColorTargetState WGPUVectorColorTargetState::Obtain() {
    WGPUVectorColorTargetState obj;
    return obj;
}

int WGPUVectorColorTargetState::size() { return vector.size(); }

void WGPUVectorColorTargetState::push_back(const WebGPUColorTargetState& attachment) { vector.push_back(attachment); }

const WebGPUColorTargetState* WGPUVectorColorTargetState::data() { return vector.data(); }

// WGPUVectorFeatureName
WGPUVectorFeatureName WGPUVectorFeatureName::Obtain() {
    WGPUVectorFeatureName obj;
    return obj;
}

int WGPUVectorFeatureName::size() { return vector.size(); }

void WGPUVectorFeatureName::push_back(const WGPUFeatureName& attachment) { vector.push_back(attachment); }

const WGPUFeatureName* WGPUVectorFeatureName::data() { return vector.data(); }

// WGPUVectorConstantEntry
WGPUVectorConstantEntry WGPUVectorConstantEntry::Obtain() {
    WGPUVectorConstantEntry obj;
    return obj;
}

int WGPUVectorConstantEntry::size() { return vector.size(); }

void WGPUVectorConstantEntry::push_back(const WebGPUConstantEntry& attachment) { vector.push_back(attachment); }

const WebGPUConstantEntry* WGPUVectorConstantEntry::data() { return vector.data(); }

// WGPUVectorVertexBufferLayout
WGPUVectorVertexBufferLayout WGPUVectorVertexBufferLayout::Obtain() {
    WGPUVectorVertexBufferLayout obj;
    return obj;
}

int WGPUVectorVertexBufferLayout::size() { return vector.size(); }

void WGPUVectorVertexBufferLayout::push_back(const WebGPUVertexBufferLayout& attachment) { vector.push_back(attachment); }

const WebGPUVertexBufferLayout* WGPUVectorVertexBufferLayout::data() { return vector.data(); }

// WGPUVectorTextureFormat
WGPUVectorTextureFormat WGPUVectorTextureFormat::Obtain() {
    WGPUVectorTextureFormat obj;
    return obj;
}

int WGPUVectorTextureFormat::size() { return vector.size(); }

WGPUTextureFormat WGPUVectorTextureFormat::get(int index) { return vector[index]; }

void WGPUVectorTextureFormat::push_back(const WGPUTextureFormat& attachment) { vector.push_back(attachment); }

const WGPUTextureFormat* WGPUVectorTextureFormat::data() { return vector.data(); }

// WGPUVectorRenderBundle
WGPUVectorRenderBundle WGPUVectorRenderBundle::Obtain() {
    WGPUVectorRenderBundle obj;
    return obj;
}

int WGPUVectorRenderBundle::size() { return vector.size(); }

void WGPUVectorRenderBundle::push_back(const WebGPURenderBundle& attachment) { vector.push_back(attachment); }

const WebGPURenderBundle* WGPUVectorRenderBundle::data() { return vector.data(); }

// WGPUVectorRenderPassColorAttachment
WGPUVectorRenderPassColorAttachment WGPUVectorRenderPassColorAttachment::Obtain() {
    WGPUVectorRenderPassColorAttachment obj;
    return obj;
}

int WGPUVectorRenderPassColorAttachment::size() { return vector.size(); }

void WGPUVectorRenderPassColorAttachment::push_back(const WebGPURenderPassColorAttachment& attachment) { vector.push_back(attachment); }

const WebGPURenderPassColorAttachment* WGPUVectorRenderPassColorAttachment::data() { return vector.data(); }

// WGPUVectorVertexAttribute
WGPUVectorVertexAttribute WGPUVectorVertexAttribute::Obtain() {
    WGPUVectorVertexAttribute obj;
    return obj;
}

int WGPUVectorVertexAttribute::size() { return vector.size(); }

void WGPUVectorVertexAttribute::push_back(const WebGPUVertexAttribute& attribute) { vector.push_back(attribute); }

const WebGPUVertexAttribute* WGPUVectorVertexAttribute::data() { return vector.data(); }

// WGPUVectorBindGroupLayout
WGPUVectorBindGroupLayout WGPUVectorBindGroupLayout::Obtain() {
    WGPUVectorBindGroupLayout obj;
    return obj;
}

int WGPUVectorBindGroupLayout::size() { return vector.size(); }

void WGPUVectorBindGroupLayout::push_back(const WebGPUBindGroupLayout& groupLayout) { vector.push_back(groupLayout); }

const WebGPUBindGroupLayout* WGPUVectorBindGroupLayout::data() { return vector.data(); }

// WGPUVectorBindGroupLayoutEntry
WGPUVectorBindGroupLayoutEntry WGPUVectorBindGroupLayoutEntry::Obtain() {
    WGPUVectorBindGroupLayoutEntry obj;
    return obj;
}

int WGPUVectorBindGroupLayoutEntry::size() { return vector.size(); }

void WGPUVectorBindGroupLayoutEntry::push_back(const WebGPUBindGroupLayoutEntry& entry) { vector.push_back(entry); }

const WebGPUBindGroupLayoutEntry* WGPUVectorBindGroupLayoutEntry::data() { return vector.data(); }

// WGPUVectorInt
WGPUVectorInt WGPUVectorInt::Obtain() {
    WGPUVectorInt obj;
    return obj;
}

int WGPUVectorInt::size() { return vector.size(); }

void WGPUVectorInt::push_back(int attachment) { vector.push_back(attachment); }

int WGPUVectorInt::get(int index) { return vector[index]; }

const uint32_t* WGPUVectorInt::data() { return vector.data(); }

// WGPUByteBuffer
bool WGPUByteBuffer::isLittleEndianHost() {
    uint16_t test = 0x0001;
    return reinterpret_cast<const uint8_t*>(&test)[0] == 0x01;
}

uint16_t WGPUByteBuffer::swapBytes(uint16_t value) {
    return (value >> 8) | (value << 8);
}

uint32_t WGPUByteBuffer::swapBytes(uint32_t value) {
    return ((value >> 24) & 0x000000FF) |
        ((value >> 8) & 0x0000FF00) |
        ((value << 8) & 0x00FF0000) |
        ((value << 24) & 0xFF000000);
}

uint64_t WGPUByteBuffer::swapBytes(uint64_t value) {
    return ((value >> 56) & 0x00000000000000FFULL) |
        ((value >> 40) & 0x000000000000FF00ULL) |
        ((value >> 24) & 0x0000000000FF0000ULL) |
        ((value >> 8) & 0x00000000FF000000ULL) |
        ((value << 8) & 0x000000FF00000000ULL) |
        ((value << 24) & 0x0000FF0000000000ULL) |
        ((value << 40) & 0x00FF000000000000ULL) |
        ((value << 56) & 0xFF00000000000000ULL);
}

WGPUByteBuffer WGPUByteBuffer::Obtain(int capacity) {
    WGPUByteBuffer obj(capacity);
    return obj;
}

int WGPUByteBuffer::getCapacity() { return buffer.size(); }

void WGPUByteBuffer::push_back(char value) { buffer.push_back(value); }

const uint8_t* WGPUByteBuffer::data() { return buffer.data(); }

void WGPUByteBuffer::order(WGPUByteOrder order) { byteOrder = order; }

void WGPUByteBuffer::put(int index, char value) {
    if (index >= _limit) {
        throw std::out_of_range("Buffer overflow");
    }
    buffer[index] = value;
}

char WGPUByteBuffer::get(int index) {
    if (index < 0 || index >= _limit) {
        throw std::out_of_range("ByteBuffer index out of bounds");
    }
    return buffer[index];
}

void WGPUByteBuffer::put(char value) {
    if (_position >= _limit) {
        throw std::out_of_range("Buffer overflow");
    }
    buffer[_position++] = value;
}

char WGPUByteBuffer::get() {
    auto value = get(_position);
    _position++;
    return value;
}

void WGPUByteBuffer::put(const uint8_t* values, int index, int size) {
    if (index < 0 || size < 0 || index + size > _limit) {
        throw std::out_of_range("Invalid index or size for ByteBuffer");
    }
    if (size == 0) return;
    std::memcpy(&buffer[index], values, size);
}

int WGPUByteBuffer::remaining() const {
    return _limit - _position;
}

void WGPUByteBuffer::position(int newPosition) {
    if (newPosition > _limit) {
        throw std::out_of_range("Invalid position");
    }
    _position = newPosition;
}

int WGPUByteBuffer::getPosition() {
    return _position;
}

void WGPUByteBuffer::limit(int newLimit) {
    if (newLimit > buffer.size()) {
        throw std::out_of_range("Invalid limit");
    }
    _limit = newLimit;
}

size_t WGPUByteBuffer::getLimit() const {
    return _limit;
}

void WGPUByteBuffer::clear() {
    _position = 0;
    _limit = buffer.size();
}

void WGPUByteBuffer::flip() {
    _limit = _position;
    _position = 0;
}

template<typename T>
void WGPUByteBuffer::putNumeric(int index, T value) {
    if (index + sizeof(T) > _limit) {
        throw std::out_of_range("Buffer overflow");
    }
    bool needsSwap = (byteOrder == WGPUByteBuffer::LittleEndian) != isLittleEndianHost();
    if constexpr (sizeof(T) == 2) {
        uint16_t intValue;
        std::memcpy(&intValue, &value, sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        std::memcpy(&buffer[index], &intValue, sizeof(T));
    }
    else if constexpr (sizeof(T) == 4) {
        uint32_t intValue;
        std::memcpy(&intValue, &value, sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        std::memcpy(&buffer[index], &intValue, sizeof(T));
    }
    else if constexpr (sizeof(T) == 8) {
        uint64_t intValue;
        std::memcpy(&intValue, &value, sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        std::memcpy(&buffer[index], &intValue, sizeof(T));
    }
    else {
        // For unsupported sizes, copy directly without swapping
        std::memcpy(&buffer[index], &value, sizeof(T));
    }
}

template<typename T>
T WGPUByteBuffer::getNumeric(int index) {
    if (index + sizeof(T) > _limit) {
        throw std::out_of_range("Buffer underflow");
    }
    bool needsSwap = (byteOrder == WGPUByteBuffer::LittleEndian) != isLittleEndianHost();
    if constexpr (sizeof(T) == 2) {
        uint16_t intValue;
        std::memcpy(&intValue, &buffer[index], sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        T value;
        std::memcpy(&value, &intValue, sizeof(T));
        return value;
    }
    else if constexpr (sizeof(T) == 4) {
        uint32_t intValue;
        std::memcpy(&intValue, &buffer[index], sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        T value;
        std::memcpy(&value, &intValue, sizeof(T));
        return value;
    }
    else if constexpr (sizeof(T) == 8) {
        uint64_t intValue;
        std::memcpy(&intValue, &buffer[index], sizeof(T));
        if (needsSwap) {
            intValue = swapBytes(intValue);
        }
        T value;
        std::memcpy(&value, &intValue, sizeof(T));
        return value;
    }
    else {
        T value;
        std::memcpy(&value, &buffer[index], sizeof(T));
        return value;
    }
}

WGPUFloatBuffer& WGPUByteBuffer::asFloatBuffer() {
    _position = 0;
    floatBuffer.parent = this;
    return floatBuffer;
}

WGPUIntBuffer& WGPUByteBuffer::asIntBuffer() {
    _position = 0;
    intBuffer.parent = this;
    return intBuffer;
}

WGPULongBuffer& WGPUByteBuffer::asLongBuffer() {
    _position = 0;
    longBuffer.parent = this;
    return longBuffer;
}

WGPUShortBuffer& WGPUByteBuffer::asShortBuffer() {
    _position = 0;
    shortBuffer.parent = this;
    return shortBuffer;
}

// WGPUFloatBuffer
WGPUByteBuffer& WGPUFloatBuffer::getByteBuffer() {
    return *parent;
}

void WGPUFloatBuffer::put(int index, float value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("FloatBuffer overflow");
    }
    parent->putNumeric(index * sizeof(float), value);
}

float WGPUFloatBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("FloatBuffer index out of bounds");
    }
    return parent->getNumeric<float>(index * sizeof(float));
}

void WGPUFloatBuffer::put(float value) {
    int floatPosition = getPosition();
    put(floatPosition, value);
    floatPosition++;
    position(floatPosition);
}

float WGPUFloatBuffer::get() {
    int floatPosition = getPosition();
    auto value = get(floatPosition);
    floatPosition++;
    position(floatPosition);
    return value;
}

void WGPUFloatBuffer::put(const float* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for FloatBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(float);
    size_t byteSize = size * sizeof(float);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == WGPUByteBuffer::LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each float
        std::vector<float> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(&temp[i]);
            *ptr = parent->swapBytes(*ptr);
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

int WGPUFloatBuffer::remaining() const {
    return getLimit() - getPosition();
}

int WGPUFloatBuffer::getCapacity() { return parent->getCapacity() / sizeof(float); }

void WGPUFloatBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for FloatBuffer");
    }
    parent->position(newPosition * sizeof(float));
}

int WGPUFloatBuffer::getPosition() const {
    return parent->getPosition() / sizeof(float);
}

void WGPUFloatBuffer::clear() {
    parent->clear();
}

void WGPUFloatBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(float));
}

int WGPUFloatBuffer::getLimit() const {
    return parent->getLimit() / sizeof(float);
}

void WGPUFloatBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPUIntBuffer
WGPUByteBuffer& WGPUIntBuffer::getByteBuffer() {
    return *parent;
}

void WGPUIntBuffer::put(int index, int value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer overflow");
    }
    parent->putNumeric(index * sizeof(int), value);
}

int WGPUIntBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer index out of bounds");
    }
    return parent->getNumeric<int>(index * sizeof(int));
}

void WGPUIntBuffer::put(int value) {
    int intPosition = getPosition();
    put(intPosition, value);
    intPosition++;
    position(intPosition);
}

int WGPUIntBuffer::get() {
    int intPosition = getPosition();
    auto value = get(intPosition);
    intPosition++;
    position(intPosition);
    return value;
}

void WGPUIntBuffer::put(const int* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for FloatBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(int);
    size_t byteSize = size * sizeof(int);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == WGPUByteBuffer::LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each float
        std::vector<int> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(&temp[i]);
            *ptr = parent->swapBytes(*ptr);
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

int WGPUIntBuffer::remaining() const {
    return getLimit() - getPosition();
}

int WGPUIntBuffer::getCapacity() { return parent->getCapacity() / sizeof(int); }

void WGPUIntBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for IntBuffer");
    }
    parent->position(newPosition * sizeof(int));
}

int WGPUIntBuffer::getPosition() const {
    return parent->getPosition() / sizeof(int);
}

void WGPUIntBuffer::clear() {
    parent->clear();
}

void WGPUIntBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(int));
}

int WGPUIntBuffer::getLimit() const {
    return parent->getLimit() / sizeof(int);
}

void WGPUIntBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPULongBuffer
WGPUByteBuffer& WGPULongBuffer::getByteBuffer() {
    return *parent;
}

void WGPULongBuffer::put(int index, long long value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer overflow");
    }
    parent->putNumeric(index * sizeof(long long), value);
}

long long WGPULongBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("IntBuffer index out of bounds");
    }
    return parent->getNumeric<long long>(index * sizeof(long long));
}

void WGPULongBuffer::put(long long value) {
    int intPosition = getPosition();
    put(intPosition, value);
    intPosition++;
    position(intPosition);
}

long long WGPULongBuffer::get() {
    int intPosition = getPosition();
    auto value = get(intPosition);
    intPosition++;
    position(intPosition);
    return value;
}

void WGPULongBuffer::put(const long long* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for FloatBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(long long);
    size_t byteSize = size * sizeof(long long);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == WGPUByteBuffer::LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each float
        std::vector<long long> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(&temp[i]);
            *ptr = parent->swapBytes(*ptr);
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

int WGPULongBuffer::remaining() const {
    return getLimit() - getPosition();
}

int WGPULongBuffer::getCapacity() { return parent->getCapacity() / sizeof(long long); }

void WGPULongBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for IntBuffer");
    }
    parent->position(newPosition * sizeof(long long));
}

int WGPULongBuffer::getPosition() const {
    return parent->getPosition() / sizeof(long long);
}

void WGPULongBuffer::clear() {
    parent->clear();
}

void WGPULongBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(long long));
}

int WGPULongBuffer::getLimit() const {
    return parent->getLimit() / sizeof(long long);
}

void WGPULongBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPUShortBuffer
WGPUByteBuffer& WGPUShortBuffer::getByteBuffer() {
    return *parent;
}

void WGPUShortBuffer::put(int index, int16_t value) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ShortBuffer overflow");
    }
    parent->putNumeric(index * sizeof(int16_t), value);
}

int16_t WGPUShortBuffer::get(int index) {
    if (index < 0 || index >= getLimit()) {
        throw std::out_of_range("ShortBuffer index out of bounds");
    }
    return parent->getNumeric<int16_t>(index * sizeof(int16_t));
}

void WGPUShortBuffer::put(int16_t value) {
    int shortPosition = getPosition();
    put(shortPosition, value);
    shortPosition++;
    position(shortPosition);
}

int16_t WGPUShortBuffer::get() {
    int shortPosition = getPosition();
    auto value = get(shortPosition);
    shortPosition++;
    position(shortPosition);
    return value;
}

void WGPUShortBuffer::put(const int16_t* values, int offset, int size) {
    if (offset < 0 || size < 0 || offset + size > getLimit()) {
        throw std::out_of_range("Invalid offset or size for ShortBuffer");
    }
    if (size == 0) return;

    size_t byteOffset = offset * sizeof(int16_t);
    size_t byteSize = size * sizeof(int16_t);
    if (byteOffset + byteSize > parent->getLimit()) {
        throw std::out_of_range("Buffer overflow");
    }

    bool needsSwap = (parent->byteOrder == WGPUByteBuffer::LittleEndian) != parent->isLittleEndianHost();
    if (!needsSwap) {
        // Direct copy if no byte-swapping is needed
        std::memcpy(&parent->buffer[byteOffset], values, byteSize);
    }
    else {
        // Copy and swap bytes for each short
        std::vector<int16_t> temp(values, values + size);
        for (int i = 0; i < size; ++i) {
            temp[i] = parent->swapBytes(static_cast<uint16_t>(temp[i]));
        }
        std::memcpy(&parent->buffer[byteOffset], temp.data(), byteSize);
    }
}

void WGPUShortBuffer::clear() {
    parent->clear();
}

void WGPUShortBuffer::limit(int newLimit) {
    parent->limit(newLimit * sizeof(float));
}

int WGPUShortBuffer::getLimit() const {
    return parent->getLimit() / sizeof(int16_t);
}

int WGPUShortBuffer::getCapacity() { return parent->getCapacity() / sizeof(int16_t); }

void WGPUShortBuffer::position(int newPosition) {
    if (newPosition < 0 || newPosition > getLimit()) {
        throw std::out_of_range("Invalid position for ShortBuffer");
    }
    parent->position(newPosition * sizeof(int16_t));
}

int WGPUShortBuffer::getPosition() const {
    return parent->getPosition() / sizeof(int16_t);
}

int WGPUShortBuffer::remaining() const {
    return getLimit() - getPosition();
}

void WGPUShortBuffer::flip() {
    int pos = getPosition();
    limit(pos);
    position(0);
}

// WGPUAndroidWindow
WGPUAndroidWindow::~WGPUAndroidWindow() {
    #ifdef __ANDROID__
        if(g_window != nullptr) {
            ANativeWindow_release(g_window);
        }
    #endif
}

void WGPUAndroidWindow::SetWindow(void * window) {
    #ifdef __ANDROID__
        g_window = reinterpret_cast<ANativeWindow * >(window);
    #endif
}

void* WGPUAndroidWindow::GetWindow() {
    #ifdef __ANDROID__
        return g_window;
    #else
        return NULL;
    #endif
}

void WGPUAndroidWindow::InitLogcat() {
    #ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Initializing Logcat redirection");
        LogcatRedirector::initialize();
    #endif
}

// WebGPUStringView
WebGPUStringView::WebGPUStringView() {}

WebGPUStringView::WebGPUStringView(WGPUStringView stringView) {
    Set(stringView);
}

WebGPUStringView::WebGPUStringView(const char* value) {
    Get().data = strdup(value);
    Get().length = strlen(value);
}

const std::string WebGPUStringView::GetString() {
    return std::string(Get().data, Get().length);
}

// WebGPUChainedStruct
void WebGPUChainedStruct::SetNext(WebGPUChainedStruct* value) {
    Get()->next = value != NULL ? value->Get() : NULL;
}

void WebGPUChainedStruct::SetSType(WGPUSType type) {
    Get()->sType = type;
}

// WebGPULimits
WebGPULimits WebGPULimits::Obtain() {
    WebGPULimits obj;
    return obj;
}

void WebGPULimits::SetMaxTextureDimension1D(int value) {
    Get().maxTextureDimension1D = value;
}

int WebGPULimits::GetMaxTextureDimension1D() {
    return static_cast<int>(Get().maxTextureDimension1D);
}

void WebGPULimits::SetMaxTextureDimension2D(int value) {
    Get().maxTextureDimension2D = value;
}

int WebGPULimits::GetMaxTextureDimension2D() {
    return static_cast<int>(Get().maxTextureDimension2D);
}

void WebGPULimits::SetMaxTextureDimension3D(int value) {
    Get().maxTextureDimension3D = value;
}

int WebGPULimits::GetMaxTextureDimension3D() {
    return static_cast<int>(Get().maxTextureDimension3D);
}

void WebGPULimits::SetMaxTextureArrayLayers(int value) {
    Get().maxTextureArrayLayers = value;
}

int WebGPULimits::GetMaxTextureArrayLayers() {
    return static_cast<int>(Get().maxTextureArrayLayers);
}

void WebGPULimits::SetMaxBindGroups(int value) {
    Get().maxBindGroups = value;
}

int WebGPULimits::GetMaxBindGroups() {
    return static_cast<int>(Get().maxBindGroups);
}

void WebGPULimits::SetMaxBindGroupsPlusVertexBuffers(int value) {
    Get().maxBindGroupsPlusVertexBuffers = value;
}

int WebGPULimits::GetMaxBindGroupsPlusVertexBuffers() {
    return static_cast<int>(Get().maxBindGroupsPlusVertexBuffers);
}

void WebGPULimits::SetMaxBindingsPerBindGroup(int value) {
    Get().maxBindingsPerBindGroup = value;
}

int WebGPULimits::GetMaxBindingsPerBindGroup() {
    return static_cast<int>(Get().maxBindingsPerBindGroup);
}

void WebGPULimits::SetMaxDynamicUniformBuffersPerPipelineLayout(int value) {
    Get().maxDynamicUniformBuffersPerPipelineLayout = value;
}

int WebGPULimits::GetMaxDynamicUniformBuffersPerPipelineLayout() {
    return static_cast<int>(Get().maxDynamicUniformBuffersPerPipelineLayout);
}

void WebGPULimits::SetMaxDynamicStorageBuffersPerPipelineLayout(int value) {
    Get().maxDynamicStorageBuffersPerPipelineLayout = value;
}

int WebGPULimits::GetMaxDynamicStorageBuffersPerPipelineLayout() {
    return static_cast<int>(Get().maxDynamicStorageBuffersPerPipelineLayout);
}

void WebGPULimits::SetMaxSampledTexturesPerShaderStage(int value) {
    Get().maxSampledTexturesPerShaderStage = value;
}

int WebGPULimits::GetMaxSampledTexturesPerShaderStage() {
    return static_cast<int>(Get().maxSampledTexturesPerShaderStage);
}

void WebGPULimits::SetMaxSamplersPerShaderStage(int value) {
    Get().maxSamplersPerShaderStage = value;
}

int WebGPULimits::GetMaxSamplersPerShaderStage() {
    return static_cast<int>(Get().maxSamplersPerShaderStage);
}

void WebGPULimits::SetMaxStorageBuffersPerShaderStage(int value) {
    Get().maxStorageBuffersPerShaderStage = value;
}

int WebGPULimits::GetMaxStorageBuffersPerShaderStage() {
    return static_cast<int>(Get().maxStorageBuffersPerShaderStage);
}

void WebGPULimits::SetMaxStorageTexturesPerShaderStage(int value) {
    Get().maxStorageTexturesPerShaderStage = value;
}

int WebGPULimits::GetMaxStorageTexturesPerShaderStage() {
    return static_cast<int>(Get().maxStorageTexturesPerShaderStage);
}

void WebGPULimits::SetMaxUniformBuffersPerShaderStage(int value) {
    Get().maxUniformBuffersPerShaderStage = value;
}

int WebGPULimits::GetMaxUniformBuffersPerShaderStage() {
    return static_cast<int>(Get().maxUniformBuffersPerShaderStage);
}

void WebGPULimits::SetMaxUniformBufferBindingSize(int value) {
    Get().maxUniformBufferBindingSize = value;
}

int WebGPULimits::GetMaxUniformBufferBindingSize() {
    return static_cast<int>(Get().maxUniformBufferBindingSize);
}

void WebGPULimits::SetMaxStorageBufferBindingSize(int value) {
    Get().maxStorageBufferBindingSize = value;
}

int WebGPULimits::GetMaxStorageBufferBindingSize() {
    return static_cast<int>(Get().maxStorageBufferBindingSize);
}

void WebGPULimits::SetMinUniformBufferOffsetAlignment(int value) {
    Get().minUniformBufferOffsetAlignment = value;
}

int WebGPULimits::GetMinUniformBufferOffsetAlignment() {
    return static_cast<int>(Get().minUniformBufferOffsetAlignment);
}

void WebGPULimits::SetMinStorageBufferOffsetAlignment(int value) {
    Get().minStorageBufferOffsetAlignment = value;
}

int WebGPULimits::GetMinStorageBufferOffsetAlignment() {
    return static_cast<int>(Get().minStorageBufferOffsetAlignment);
}

void WebGPULimits::SetMaxVertexBuffers(int value) {
    Get().maxVertexBuffers = value;
}

int WebGPULimits::GetMaxVertexBuffers() {
    return static_cast<int>(Get().maxVertexBuffers);
}

void WebGPULimits::SetMaxBufferSize(int value) {
    Get().maxBufferSize = value;
}

int WebGPULimits::GetMaxBufferSize() {
    return static_cast<int>(Get().maxBufferSize);
}

void WebGPULimits::SetMaxVertexAttributes(int value) {
    Get().maxVertexAttributes = value;
}

int WebGPULimits::GetMaxVertexAttributes() {
    return static_cast<int>(Get().maxVertexAttributes);
}

void WebGPULimits::SetMaxVertexBufferArrayStride(int value) {
    Get().maxVertexBufferArrayStride = value;
}

int WebGPULimits::GetMaxVertexBufferArrayStride() {
    return static_cast<int>(Get().maxVertexBufferArrayStride);
}

void WebGPULimits::SetMaxInterStageShaderVariables(int value) {
    Get().maxInterStageShaderVariables = value;
}

int WebGPULimits::GetMaxInterStageShaderVariables() {
    return static_cast<int>(Get().maxInterStageShaderVariables);
}

void WebGPULimits::SetMaxColorAttachments(int value) {
    Get().maxColorAttachments = value;
}

int WebGPULimits::GetMaxColorAttachments() {
    return static_cast<int>(Get().maxColorAttachments);
}

void WebGPULimits::SetMaxColorAttachmentBytesPerSample(int value) {
    Get().maxColorAttachmentBytesPerSample = value;
}

int WebGPULimits::GetMaxColorAttachmentBytesPerSample() {
    return static_cast<int>(Get().maxColorAttachmentBytesPerSample);
}

void WebGPULimits::SetMaxComputeWorkgroupStorageSize(int value) {
    Get().maxComputeWorkgroupStorageSize = value;
}

int WebGPULimits::GetMaxComputeWorkgroupStorageSize() {
    return static_cast<int>(Get().maxComputeWorkgroupStorageSize);
}

void WebGPULimits::SetMaxComputeInvocationsPerWorkgroup(int value) {
    Get().maxComputeInvocationsPerWorkgroup = value;
}

int WebGPULimits::GetMaxComputeInvocationsPerWorkgroup() {
    return static_cast<int>(Get().maxComputeInvocationsPerWorkgroup);
}

void WebGPULimits::SetMaxComputeWorkgroupSizeX(int value) {
    Get().maxComputeWorkgroupSizeX = value;
}

int WebGPULimits::GetMaxComputeWorkgroupSizeX() {
    return static_cast<int>(Get().maxComputeWorkgroupSizeX);
}

void WebGPULimits::SetMaxComputeWorkgroupSizeY(int value) {
    Get().maxComputeWorkgroupSizeY = value;
}

int WebGPULimits::GetMaxComputeWorkgroupSizeY() {
    return static_cast<int>(Get().maxComputeWorkgroupSizeY);
}

void WebGPULimits::SetMaxComputeWorkgroupSizeZ(int value) {
    Get().maxComputeWorkgroupSizeZ = value;
}

int WebGPULimits::GetMaxComputeWorkgroupSizeZ() {
    return static_cast<int>(Get().maxComputeWorkgroupSizeZ);
}

void WebGPULimits::SetMaxComputeWorkgroupsPerDimension(int value) {
    Get().maxComputeWorkgroupsPerDimension = value;
}

int WebGPULimits::GetMaxComputeWorkgroupsPerDimension() {
    return static_cast<int>(Get().maxComputeWorkgroupsPerDimension);
}

// WebGPUBufferBindingLayout
WebGPUBufferBindingLayout WebGPUBufferBindingLayout::Obtain() {
    WebGPUBufferBindingLayout obj;
    return obj;
}

void WebGPUBufferBindingLayout::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUBufferBindingLayout::SetType(WGPUBufferBindingType type) {
    Get()->type = type;
}

void WebGPUBufferBindingLayout::SetHasDynamicOffset(int hasDynamicOffset) {
    Get()->hasDynamicOffset = hasDynamicOffset;
}

void WebGPUBufferBindingLayout::SetMinBindingSize(int minBindingSize) {
    Get()->minBindingSize = minBindingSize;
}

// WebGPUSamplerBindingLayout
WebGPUSamplerBindingLayout WebGPUSamplerBindingLayout::Obtain() {
    WebGPUSamplerBindingLayout obj;
    return obj;
}

void WebGPUSamplerBindingLayout::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUSamplerBindingLayout::SetType(WGPUSamplerBindingType type) {
    Get()->type = type;
}

// WebGPUTextureBindingLayout
WebGPUTextureBindingLayout WebGPUTextureBindingLayout::Obtain() {
    WebGPUTextureBindingLayout obj;
    return obj;
}

void WebGPUTextureBindingLayout::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUTextureBindingLayout::SetSampleType(WGPUTextureSampleType sampleType) {
    Get()->sampleType = sampleType;
}

void WebGPUTextureBindingLayout::SetViewDimension(WGPUTextureViewDimension viewDimension) {
    Get()->viewDimension = viewDimension;
}

void WebGPUTextureBindingLayout::SetMultisampled(int multisampled) {
    Get()->multisampled = multisampled;
}

// WebGPUStorageTextureBindingLayout
WebGPUStorageTextureBindingLayout WebGPUStorageTextureBindingLayout::Obtain() {
    WebGPUStorageTextureBindingLayout obj;
    return obj;
}

void WebGPUStorageTextureBindingLayout::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUStorageTextureBindingLayout::SetAccess(WGPUStorageTextureAccess access) {
    Get()->access = access;
}

void WebGPUStorageTextureBindingLayout::SetFormat(WGPUTextureFormat format) {
    Get()->format = format;
}

void WebGPUStorageTextureBindingLayout::SetViewDimension(WGPUTextureViewDimension viewDimension) {
    Get()->viewDimension = viewDimension;
}

// WebGPUBindGroupLayoutEntry
WebGPUBindGroupLayoutEntry WebGPUBindGroupLayoutEntry::Obtain() {
    WebGPUBindGroupLayoutEntry obj;
    return obj;
}

void WebGPUBindGroupLayoutEntry::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUBindGroupLayoutEntry::SetBinding(int binding) {
    Get().binding = binding;
}

void WebGPUBindGroupLayoutEntry::SetVisibility(WGPUShaderStage visibility) {
    Get().visibility = visibility;
}

void WebGPUBindGroupLayoutEntry::SetBuffer(WebGPUBufferBindingLayout* buffer) {
    Get().buffer = *buffer->Get();
}

void WebGPUBindGroupLayoutEntry::SetSampler(WebGPUSamplerBindingLayout* sampler) {
    Get().sampler = *sampler->Get();
}

void WebGPUBindGroupLayoutEntry::SetTexture(WebGPUTextureBindingLayout* texture) {
    Get().texture = *texture->Get();
}

void WebGPUBindGroupLayoutEntry::SetStorageTexture(WebGPUStorageTextureBindingLayout* storageTexture) {
    Get().storageTexture = *storageTexture->Get();
}

WebGPUBufferBindingLayout WebGPUBindGroupLayoutEntry::GetBuffer() {
    WebGPUBufferBindingLayout temp(&Get().buffer);
    return temp;
}

WebGPUSamplerBindingLayout WebGPUBindGroupLayoutEntry::GetSampler() {
    WebGPUSamplerBindingLayout temp(&Get().sampler);
    return temp;
}

WebGPUStorageTextureBindingLayout WebGPUBindGroupLayoutEntry::GetStorageTexture() {
    WebGPUStorageTextureBindingLayout temp(&Get().storageTexture);
    return temp;
}

WebGPUTextureBindingLayout WebGPUBindGroupLayoutEntry::GetTexture() {
    WebGPUTextureBindingLayout temp(&Get().texture);
    return temp;
}

// WebGPURequestAdapterOptions
WebGPURequestAdapterOptions WebGPURequestAdapterOptions::Obtain() {
    WebGPURequestAdapterOptions obj;
    return obj;
}

void WebGPURequestAdapterOptions::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPURequestAdapterOptions::SetFeatureLevel(WGPUFeatureLevel featureLevel) {
    Get().featureLevel = featureLevel;
}

void WebGPURequestAdapterOptions::SetPowerPreference(WGPUPowerPreference powerPreference) {
    Get().powerPreference = powerPreference;
}

void WebGPURequestAdapterOptions::SetBackendType(WGPUBackendType backendType) {
    Get().backendType = backendType;
}

void WebGPURequestAdapterOptions::SetCompatibleSurface(WebGPUSurface* compatibleSurface) {
    Get().compatibleSurface = compatibleSurface->Get();
}

// WebGPUAdapterInfo
WebGPUAdapterInfo WebGPUAdapterInfo::Obtain() {
    WebGPUAdapterInfo obj;
    return obj;
}

std::string WebGPUAdapterInfo::GetVendor() {
    WebGPUStringView stringView(Get().vendor);
    return stringView.GetString();
}

int WebGPUAdapterInfo::GetVendorID() {
    return Get().vendorID;
}

std::string WebGPUAdapterInfo::GetArchitecture() {
    WebGPUStringView stringView(Get().architecture);
    return stringView.GetString();
}

std::string WebGPUAdapterInfo::GetDevice() {
    WebGPUStringView stringView(Get().device);
    return stringView.GetString();
}

int WebGPUAdapterInfo::GetDeviceID() {
    return Get().deviceID;
}

std::string WebGPUAdapterInfo::GetDescription() {
    WebGPUStringView stringView(Get().description);
    return stringView.GetString();
}

WGPUBackendType WebGPUAdapterInfo::GetBackendType() {
    return Get().backendType;
}

WGPUAdapterType WebGPUAdapterInfo::GetAdapterType() {
    return Get().adapterType;
}

// WebGPUFutureWaitInfo
WebGPUFutureWaitInfo WebGPUFutureWaitInfo::Obtain() {
    WebGPUFutureWaitInfo obj;
    return obj;
}

void WebGPUFutureWaitInfo::SetFuture(WebGPUFuture* future) {
    Get().future = future != NULL ? future->Get() : WGPUFuture();
}

// WebGPUVertexAttribute
WebGPUVertexAttribute WebGPUVertexAttribute::Obtain() {
    WebGPUVertexAttribute obj;
    return obj;
}

void WebGPUVertexAttribute::SetFormat(WGPUVertexFormat format) {
    Get().format = format;
}

void WebGPUVertexAttribute::SetOffset(int offset) {
    Get().offset = offset;
}

void WebGPUVertexAttribute::SetShaderLocation(int shaderLocation) {
    Get().shaderLocation = shaderLocation;
}

// WebGPUVertexBufferLayout
WebGPUVertexBufferLayout WebGPUVertexBufferLayout::Obtain() {
    WebGPUVertexBufferLayout obj;
    return obj;
}

void WebGPUVertexBufferLayout::SetAttributes(WGPUVectorVertexAttribute* values) {
    if(values != NULL) {
        Get().attributeCount = values->size();
        Get().attributes = reinterpret_cast<const WGPUVertexAttribute*>(values->data());
    }
    else {
        Get().attributeCount = 0;
        Get().attributes = NULL;
    }
}

void WebGPUVertexBufferLayout::SetArrayStride(int offset) {
    Get().arrayStride = offset;
}

void WebGPUVertexBufferLayout::SetStepMode(WGPUVertexStepMode stepMode) {
    Get().stepMode = stepMode;
}

// WebGPUVertexState
void WebGPUVertexState::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUVertexState::SetModule(WebGPUShaderModule* shaderModule) {
    Get()->module = shaderModule != NULL ? shaderModule->Get() : NULL;
}

void WebGPUVertexState::SetEntryPoint(const char* value) {
    WebGPUStringView stringView(value);
    Get()->entryPoint = stringView.Get();
}

void WebGPUVertexState::SetConstants(WGPUVectorConstantEntry* values) {
    if(values != NULL) {
        Get()->constantCount = values->size();
        Get()->constants = reinterpret_cast<const WGPUConstantEntry*>(values->data());
    }
    else {
        Get()->constantCount = 0;
        Get()->constants = NULL;
    }
}

void WebGPUVertexState::SetBuffers(WGPUVectorVertexBufferLayout* values) {
    if(values != NULL) {
        Get()->bufferCount = values->size();
        Get()->buffers = reinterpret_cast<const WGPUVertexBufferLayout*>(values->data());
    }
    else {
        Get()->bufferCount = 0;
        Get()->buffers = NULL;
    }
}

// WebGPUShaderSourceWGSL
WebGPUShaderSourceWGSL WebGPUShaderSourceWGSL::Obtain() {
    WebGPUShaderSourceWGSL obj;
    return obj;
}

void WebGPUShaderSourceWGSL::SetCode(const char* value) {
    WebGPUStringView stringView(value);
    Get().code = stringView.Get();
}

WebGPUChainedStruct WebGPUShaderSourceWGSL::GetChain() {
    WGPUChainedStruct* wgpuChain = &Get().chain;
    WebGPUChainedStruct chain;
    chain.Set(wgpuChain);
    return chain;
}

// WebGPUBlendComponent
void WebGPUBlendComponent::SetOperation(WGPUBlendOperation operation) {
    Get()->operation = operation;
}

WGPUBlendOperation WebGPUBlendComponent::GetOperation() {
    return Get()->operation;
}

void WebGPUBlendComponent::SetSrcFactor(WGPUBlendFactor factor) {
    Get()->srcFactor = factor;
}

WGPUBlendFactor WebGPUBlendComponent::GetSrcFactor() {
    return Get()->srcFactor;
}

void WebGPUBlendComponent::SetDstFactor(WGPUBlendFactor factor) {
    Get()->dstFactor = factor;
}

WGPUBlendFactor WebGPUBlendComponent::GetDstFactor() {
    return Get()->dstFactor;
}

// WebGPUBlendState
WebGPUBlendState WebGPUBlendState::Obtain() {
    WebGPUBlendState obj;
    return obj;
}

WebGPUBlendComponent WebGPUBlendState::GetColor() {
    WebGPUBlendComponent temp;
    temp.Set(&Get().color);
    return temp;
}

WebGPUBlendComponent WebGPUBlendState::GetAlpha() {
    WebGPUBlendComponent temp;
    temp.Set(&Get().alpha);
    return temp;
}

// WebGPUColorTargetState
WebGPUColorTargetState WebGPUColorTargetState::Obtain() {
    WebGPUColorTargetState obj;
    return obj;
}

void WebGPUColorTargetState::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUColorTargetState::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void WebGPUColorTargetState::SetBlend(WebGPUBlendState* blendState) {
    Get().blend = reinterpret_cast<const WGPUBlendState*>(blendState);
}

void WebGPUColorTargetState::SetWriteMask(WGPUColorWriteMask writeMask) {
    Get().writeMask = writeMask;
}

// WebGPUFragmentState
WebGPUFragmentState WebGPUFragmentState::Obtain() {
    WebGPUFragmentState obj;
    return obj;
}

void WebGPUFragmentState::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUFragmentState::SetEntryPoint(const char* value) {
    WebGPUStringView stringView(value);
    Get().entryPoint = stringView.Get();
}

void WebGPUFragmentState::SetTargets(WGPUVectorColorTargetState* values) {
    if(values != NULL) {
        Get().targetCount = values->size();
        Get().targets = reinterpret_cast<const WGPUColorTargetState*>(values->data());
    }
    else {
        Get().targetCount = 0;
        Get().targets = NULL;
    }
}

void WebGPUFragmentState::SetModule(WebGPUShaderModule* shaderModule) {
    Get().module = shaderModule != NULL ? shaderModule->Get() : NULL;
}

void WebGPUFragmentState::SetConstants(WGPUVectorConstantEntry* values) {
    if(values != NULL) {
        Get().constantCount = values->size();
        Get().constants = reinterpret_cast<const WGPUConstantEntry*>(values->data());
    }
    else {
        Get().constantCount = 0;
        Get().constants = NULL;
    }
}

// WebGPUPrimitiveState
void WebGPUPrimitiveState::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUPrimitiveState::SetTopology(WGPUPrimitiveTopology value) {
    Get()->topology = value;
}

void WebGPUPrimitiveState::SetStripIndexFormat(WGPUIndexFormat value) {
    Get()->stripIndexFormat = value;
}

void WebGPUPrimitiveState::SetFrontFace(WGPUFrontFace value) {
    Get()->frontFace = value;
}

void WebGPUPrimitiveState::SetCullMode(WGPUCullMode value) {
    Get()->cullMode = value;
}

// WebGPUStencilFaceState
void WebGPUStencilFaceState::SetCompare(WGPUCompareFunction compare) {
    Get()->compare = compare;
}

void WebGPUStencilFaceState::SetFailOp(WGPUStencilOperation failOp) {
    Get()->failOp = failOp;
}

void WebGPUStencilFaceState::SetDepthFailOp(WGPUStencilOperation depthFailOp) {
    Get()->depthFailOp = depthFailOp;
}

void WebGPUStencilFaceState::SetPassOp(WGPUStencilOperation passOp) {
    Get()->passOp = passOp;
}

// WebGPUDepthStencilState
WebGPUDepthStencilState WebGPUDepthStencilState::Obtain() {
    WebGPUDepthStencilState obj;
    return obj;
}

void WebGPUDepthStencilState::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUDepthStencilState::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void WebGPUDepthStencilState::SetDepthWriteEnabled(WGPUOptionalBool depthWriteEnabled) {
    Get().depthWriteEnabled = depthWriteEnabled;
}

void WebGPUDepthStencilState::SetDepthCompare(WGPUCompareFunction depthCompare) {
    Get().depthCompare = depthCompare;
}

void WebGPUDepthStencilState::SetDepthBiasSlopeScale(float depthBiasSlopeScale) {
    Get().depthBiasSlopeScale = depthBiasSlopeScale;
}

void WebGPUDepthStencilState::SetDepthBiasClamp(float depthBiasClamp) {
    Get().depthBiasClamp = depthBiasClamp;
}

void WebGPUDepthStencilState::SetStencilReadMask(int stencilReadMask) {
    Get().stencilReadMask = stencilReadMask;
}

void WebGPUDepthStencilState::SetStencilWriteMask(int stencilWriteMask) {
    Get().stencilWriteMask = stencilWriteMask;
}

void WebGPUDepthStencilState::SetDepthBias(int depthBias) {
    Get().depthBias = depthBias;
}

WebGPUStencilFaceState WebGPUDepthStencilState::GetStencilFront() {
    WebGPUStencilFaceState temp;
    temp.Set(&Get().stencilFront);
    return temp;
}

WebGPUStencilFaceState WebGPUDepthStencilState::GetStencilBack() {
    WebGPUStencilFaceState temp;
    temp.Set(&Get().stencilBack);
    return temp;
}

// WebGPUMultisampleState
void WebGPUMultisampleState::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUMultisampleState::SetCount(int count) {
    Get()->count = count;
}

void WebGPUMultisampleState::SetMask(int mask) {
    Get()->mask = mask;
}

void WebGPUMultisampleState::SetAlphaToCoverageEnabled(bool alphaToCoverageEnabled) {
    Get()->alphaToCoverageEnabled = alphaToCoverageEnabled;
}

// WebGPUColor
WebGPUColor WebGPUColor::Obtain() {
    WebGPUColor obj;
    return obj;
}

void WebGPUColor::SetColor(float r, float g, float b, float a) {
    Get()->r = r;
    Get()->g = g;
    Get()->b = b;
    Get()->a = a;
}

float WebGPUColor::GetR() {
    return Get()->r;
}

float WebGPUColor::GetG() {
    return Get()->g;
}

float WebGPUColor::GetB() {
    return Get()->b;
}

float WebGPUColor::GetA() {
    return Get()->a;
}

void WebGPUColor::SetR(float value) {
    Get()->r = value;
}

void WebGPUColor::SetG(float value) {
    Get()->g = value;
}

void WebGPUColor::SetB(float value) {
    Get()->b = value;
}

void WebGPUColor::SetA(float value) {
    Get()->a = value;
}

// WebGPUSupportedFeatures
WebGPUSupportedFeatures WebGPUSupportedFeatures::Obtain() {
    WebGPUSupportedFeatures obj;
    return obj;
}

int WebGPUSupportedFeatures::GetFeatureCount() {
    return Get().featureCount;
}

WGPUFeatureName WebGPUSupportedFeatures::GetFeatureAt(long index) {
    return Get().features[index];
}

// WebGPURenderPassDepthStencilAttachment
WebGPURenderPassDepthStencilAttachment WebGPURenderPassDepthStencilAttachment::Obtain() {
    WebGPURenderPassDepthStencilAttachment obj;
    return obj;
}

void WebGPURenderPassDepthStencilAttachment::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
// TODO dont exist in wgpu-native
//    Get().nextInChain = chainedStruct ? chainedStruct->Get() : nullptr;
}

void WebGPURenderPassDepthStencilAttachment::SetView(WebGPUTextureView* textureView) {
    Get().view = textureView ? textureView->Get() : nullptr;
}

void WebGPURenderPassDepthStencilAttachment::SetDepthLoadOp(WGPULoadOp loadOp) {
    Get().depthLoadOp = loadOp;
}

void WebGPURenderPassDepthStencilAttachment::SetDepthStoreOp(WGPUStoreOp storeOp) {
    Get().depthStoreOp = storeOp;
}

void WebGPURenderPassDepthStencilAttachment::SetDepthClearValue(float depthClearValue) {
    Get().depthClearValue = depthClearValue;
}

void WebGPURenderPassDepthStencilAttachment::SetDepthReadOnly(bool depthReadOnly) {
    Get().depthReadOnly = depthReadOnly;
}

void WebGPURenderPassDepthStencilAttachment::SetStencilLoadOp(WGPULoadOp loadOp) {
    Get().stencilLoadOp = loadOp;
}

void WebGPURenderPassDepthStencilAttachment::SetStencilStoreOp(WGPUStoreOp storeOp) {
    Get().stencilStoreOp = storeOp;
}

void WebGPURenderPassDepthStencilAttachment::SetStencilClearValue(int stencilClearValue) {
    Get().stencilClearValue = stencilClearValue;
}

void WebGPURenderPassDepthStencilAttachment::SetStencilReadOnly(bool stencilReadOnly) {
    Get().stencilReadOnly = stencilReadOnly;
}

// WebGPURenderPassTimestampWrites
WebGPURenderPassTimestampWrites WebGPURenderPassTimestampWrites::Obtain() {
    WebGPURenderPassTimestampWrites obj;
    return obj;
}

void WebGPURenderPassTimestampWrites::SetQuerySet(WebGPUQuerySet* value) {
    Get().querySet = value->Get();
}

void WebGPURenderPassTimestampWrites::SetBeginningOfPassWriteIndex(int value) {
    Get().beginningOfPassWriteIndex = value;
}

void WebGPURenderPassTimestampWrites::SetEndOfPassWriteIndex(int value) {
    Get().endOfPassWriteIndex = value;
}

// WebGPUComputePassTimestampWrites
WebGPUComputePassTimestampWrites WebGPUComputePassTimestampWrites::Obtain() {
    WebGPUComputePassTimestampWrites obj;
    return obj;
}

void WebGPUComputePassTimestampWrites::SetQuerySet(WebGPUQuerySet* value) {
    Get().querySet = value->Get();
}

void WebGPUComputePassTimestampWrites::SetBeginningOfPassWriteIndex(int value) {
    Get().beginningOfPassWriteIndex = value;
}

void WebGPUComputePassTimestampWrites::SetEndOfPassWriteIndex(int value) {
    Get().endOfPassWriteIndex = value;
}

// WebGPUOrigin3D
WebGPUOrigin3D WebGPUOrigin3D::Obtain() {
    WebGPUOrigin3D obj;
    return obj;
}

void WebGPUOrigin3D::Set(int x, int y, int z) {
    Get()->x = x;
    Get()->y = y;
    Get()->z = z;
}

void WebGPUOrigin3D::SetX(int value) {
    Get()->x = value;
}

void WebGPUOrigin3D::SetY(int value) {
    Get()->y = value;
}

void WebGPUOrigin3D::SetZ(int value) {
    Get()->z = value;
}

// WebGPUTexelCopyTextureInfo
WebGPUTexelCopyTextureInfo WebGPUTexelCopyTextureInfo::Obtain() {
    WebGPUTexelCopyTextureInfo obj;
    return obj;
}

void WebGPUTexelCopyTextureInfo::SetTexture(WebGPUTexture* texture) {
    Get().texture = texture->Get();
}

void WebGPUTexelCopyTextureInfo::SetMipLevel(int mipLevel) {
    Get().mipLevel = mipLevel;
}

WebGPUOrigin3D WebGPUTexelCopyTextureInfo::GetOrigin() {
    WebGPUOrigin3D temp(&Get().origin);
    return temp;
}

void WebGPUTexelCopyTextureInfo::SetAspect(WGPUTextureAspect aspect) {
    Get().aspect = aspect;
}

// WebGPUExtent3D
WebGPUExtent3D WebGPUExtent3D::Obtain() {
    WebGPUExtent3D obj;
    return obj;
}

void WebGPUExtent3D::SetWidth(int width) {
    Get()->width = width;
}

void WebGPUExtent3D::SetHeight(int height) {
    Get()->height = height;
}

void WebGPUExtent3D::SetDepthOrArrayLayers(int depthOrArrayLayers) {
    Get()->depthOrArrayLayers = depthOrArrayLayers;
}

// WebGPUTexelCopyBufferLayout
WebGPUTexelCopyBufferLayout WebGPUTexelCopyBufferLayout::Obtain() {
    WebGPUTexelCopyBufferLayout obj;
    return obj;
}

void WebGPUTexelCopyBufferLayout::SetOffset(int offset) {
    Get()->offset = offset;
}

void WebGPUTexelCopyBufferLayout::SetBytesPerRow(int bytesPerRow) {
    Get()->bytesPerRow = bytesPerRow;
}

void WebGPUTexelCopyBufferLayout::SetRowsPerImage(int rowsPerImage) {
    Get()->rowsPerImage = rowsPerImage;
}

// WebGPUTexelCopyBufferInfo
WebGPUTexelCopyBufferInfo WebGPUTexelCopyBufferInfo::Obtain() {
    WebGPUTexelCopyBufferInfo obj;
    return obj;
}

WebGPUTexelCopyBufferLayout WebGPUTexelCopyBufferInfo::GetLayout() {
    WebGPUTexelCopyBufferLayout temp(&Get().layout);
    return temp;
}

WebGPUBuffer WebGPUTexelCopyBufferInfo::GetBuffer() {
    WebGPUBuffer temp;
    temp.Set(Get().buffer);
    return temp;
}

void WebGPUTexelCopyBufferInfo::SetBuffer(WebGPUBuffer* buffer) {
    Get().buffer = buffer->Get();
}

// WebGPUSurfaceConfiguration
WebGPUSurfaceConfiguration WebGPUSurfaceConfiguration::Obtain() {
    WebGPUSurfaceConfiguration temp;
    return temp;
}

void WebGPUSurfaceConfiguration::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUSurfaceConfiguration::SetWidth(long width) {
    Get().width = width;
}

void WebGPUSurfaceConfiguration::SetHeight(long height) {
    Get().height = height;
}

void WebGPUSurfaceConfiguration::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void WebGPUSurfaceConfiguration::SetViewFormatCount(long value) {
    Get().viewFormatCount = value;
}

void WebGPUSurfaceConfiguration::SetViewFormats(WGPUVectorTextureFormat* values) {
    if(values != NULL) {
        Get().viewFormatCount = values->size();
        Get().viewFormats = reinterpret_cast<const WGPUTextureFormat*>(values->data());
    }
    else {
        Get().viewFormatCount = 0;
        Get().viewFormats = NULL;
    }
}

void WebGPUSurfaceConfiguration::SetUsage(WGPUTextureUsage usage) {
    Get().usage = usage;
}

void WebGPUSurfaceConfiguration::SetDevice(WebGPUDevice* device) {
    Get().device = device != NULL ? device->Get() : NULL;
}

void WebGPUSurfaceConfiguration::SetPresentMode(WGPUPresentMode presentMode) {
    Get().presentMode = presentMode;
}

void WebGPUSurfaceConfiguration::SetAlphaMode(WGPUCompositeAlphaMode alphaMode) {
    Get().alphaMode = alphaMode;
}

// WebGPUSurfaceCapabilities
WebGPUSurfaceCapabilities WebGPUSurfaceCapabilities::Obtain() {
    WebGPUSurfaceCapabilities obj;
    return obj;
}

WGPUVectorTextureFormat WebGPUSurfaceCapabilities::GetFormats() {
    WGPUVectorTextureFormat temp(Get().formats, Get().formatCount);
    return temp;
}

// WebGPURenderPassColorAttachment
WebGPURenderPassColorAttachment::WebGPURenderPassColorAttachment() {
    SetDepthSlice(WGPU_DEPTH_SLICE_UNDEFINED); // Required for emscripten
}

WebGPURenderPassColorAttachment WebGPURenderPassColorAttachment::Obtain() {
    WebGPURenderPassColorAttachment renderPassColorAttachment;
    return renderPassColorAttachment;
}

void WebGPURenderPassColorAttachment::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct ? chainedStruct->Get() : nullptr;
}

void WebGPURenderPassColorAttachment::SetResolveTarget(WebGPUTextureView* textureView) {
    Get().resolveTarget = textureView == NULL ? NULL : textureView->Get();
}

void WebGPURenderPassColorAttachment::SetView(WebGPUTextureView* textureView) {
    Get().view = textureView == NULL ? NULL : textureView->Get();
}

void WebGPURenderPassColorAttachment::SetLoadOp(WGPULoadOp loadOp) {
    Get().loadOp = loadOp;
}

void WebGPURenderPassColorAttachment::SetStoreOp(WGPUStoreOp storeOp) {
    Get().storeOp = storeOp;
}

void WebGPURenderPassColorAttachment::SetDepthSlice(int depthSlice) {
    Get().depthSlice = depthSlice;
}

WebGPUColor WebGPURenderPassColorAttachment::GetClearValue() {
    WebGPUColor temp(&Get().clearValue);
    return temp;
}

// WebGPUSurfaceTexture
WebGPUSurfaceTexture WebGPUSurfaceTexture::Obtain() {
    WebGPUSurfaceTexture obj;
    return obj;
}

void WebGPUSurfaceTexture::GetTexture(WebGPUTexture* tex) {
    tex->Set(Get().texture);
}

WGPUSurfaceGetCurrentTextureStatus WebGPUSurfaceTexture::GetStatus() {
    return Get().status;
}

// WGPU
WGPUPlatformType WGPU::GetPlatformType() {
    #if __EMSCRIPTEN__
        return WGPUPlatformType::WGPU_Web;
    #elif __ANDROID__
        return WGPUPlatformType::WGPU_Android;
    #elif TARGET_OS_IPHONE
        return WGPUPlatformType::WGPU_iOS;
    #elif _WIN32
        return WGPUPlatformType::WGPU_Windows;
    #elif __linux__
        return WGPUPlatformType::WGPU_Linux;
    #elif TARGET_OS_MAC
        return WGPUPlatformType::WGPU_Mac;
    #else
        return WGPUPlatformType::WGPU_Unknown;
    #endif
}

WebGPUInstance WGPU::CreateInstance(WebGPUInstanceDescriptor* descriptor) {
    WebGPUInstance instance;
    instance.Set(wgpuCreateInstance(descriptor != NULL ? &descriptor->Get() : NULL));
    return instance;
}

STBImage* WGPU::loadImage(WGPUByteBuffer* buffer, int desiredChannels) {
    int32_t width, height, format;
    unsigned char* pixels = stbi_load_from_memory(buffer->data(), buffer->getLimit(), &width, &height, &format, desiredChannels);
    if (pixels == NULL)
        return NULL;

    int bufferFormat = format;
    if(desiredChannels > 0) {
        bufferFormat = desiredChannels;
    }

    STBImage* stbImage = new STBImage();
    stbImage->pixel = WGPUByteBuffer(pixels, width * height * bufferFormat);
    stbi_image_free(pixels);
    stbImage->width = width;
    stbImage->height = height;
    stbImage->format = format;
    return stbImage;
}

// WebGPUBindGroupEntry
WebGPUBindGroupEntry WebGPUBindGroupEntry::Obtain() {
    WebGPUBindGroupEntry obj;
    return obj;
}

void WebGPUBindGroupEntry::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct ? chainedStruct->Get() : nullptr;
}

void WebGPUBindGroupEntry::SetBinding(int binding) {
    Get().binding = binding;
}

void WebGPUBindGroupEntry::SetBuffer(WebGPUBuffer* buffer) {
    Get().buffer = buffer ? buffer->Get() : nullptr;
}

void WebGPUBindGroupEntry::SetOffset(int offset) {
    Get().offset = offset;
}

void WebGPUBindGroupEntry::SetSize(int size) {
    Get().size = size;
}

void WebGPUBindGroupEntry::SetSampler(WebGPUSampler* sampler) {
    Get().sampler = sampler ? sampler->Get() : nullptr;
}

void WebGPUBindGroupEntry::SetTextureView(WebGPUTextureView* textureView) {
    Get().textureView = textureView ? textureView->Get() : nullptr;
}

// WebGPUInstanceCapabilities
//void WebGPUInstanceCapabilities::SetTimedWaitAnyEnable(bool timedWaitAnyEnable) {
//    Get()->timedWaitAnyEnable = timedWaitAnyEnable;
//}
//
//void WebGPUInstanceCapabilities::SetTimedWaitAnyMaxCount(int timedWaitAnyMaxCount) {
//    Get()->timedWaitAnyMaxCount = timedWaitAnyMaxCount;
//}

// WebGPURenderPassMaxDrawCount
WebGPURenderPassMaxDrawCount WebGPURenderPassMaxDrawCount::Obtain() {
    WebGPURenderPassMaxDrawCount obj;
    return obj;
}

void WebGPURenderPassMaxDrawCount::SetMaxDrawCount(int maxDrawCount) {
    Get().maxDrawCount = maxDrawCount;
}

int WebGPURenderPassMaxDrawCount::GetMaxDrawCount() {
    return Get().maxDrawCount;
}

// WebGPUShaderSourceSPIRV
WebGPUShaderSourceSPIRV WebGPUShaderSourceSPIRV::Obtain() {
    WebGPUShaderSourceSPIRV obj;
    return obj;
}

//void WebGPUShaderSourceSPIRV::SetCode(const uint32_t* code, size_t codeSize) {
//    this->code.assign(code, code + codeSize);
//    Get().codeSize = codeSize;
//    Get().code = this->code.data();
//}
//
//WebGPUChainedStruct WebGPUShaderSourceSPIRV::GetChain() {
//    WebGPUChainedStruct chain;
//    chain.Set(&Get().chain);
//    return chain;
//}

// WebGPUShaderSourceSPIRV
WebGPUSupportedWGSLLanguageFeatures WebGPUSupportedWGSLLanguageFeatures::Obtain() {
    WebGPUSupportedWGSLLanguageFeatures obj;
    return obj;
}

int WebGPUSupportedWGSLLanguageFeatures::GetFeatureCount() {
    return Get().featureCount;
}

WGPUWGSLLanguageFeatureName WebGPUSupportedWGSLLanguageFeatures::GetFeatureAt(int index) {
    return (index < Get().featureCount) ? Get().features[index] : WGPUWGSLLanguageFeatureName(0);
}

// WebGPUCompilationMessage
WebGPUCompilationMessage WebGPUCompilationMessage::Obtain() {
    WebGPUCompilationMessage obj;
    return obj;
}

std::string WebGPUCompilationMessage::GetMessage() {
    return std::string(Get().message.data, Get().message.length);
}

WGPUCompilationMessageType WebGPUCompilationMessage::GetType() {
    return Get().type;
}

int WebGPUCompilationMessage::GetLineNum() {
    return Get().lineNum;
}

int WebGPUCompilationMessage::GetLinePos() {
    return Get().linePos;
}

int WebGPUCompilationMessage::GetOffset() {
    return Get().offset;
}

int WebGPUCompilationMessage::GetLength() {
    return Get().length;
}

// WebGPUCompilationInfo
int WebGPUCompilationInfo::GetMessageCount() {
    return Get().messageCount;
}

WebGPUCompilationMessage WebGPUCompilationInfo::GetMessage(int index) {
    int size = GetMessageCount();
    if(index >= 0 && index < size) {
        WebGPUCompilationMessage temp;
        temp.Set(Get().messages[index]);
        return temp;
    }
    return WebGPUCompilationMessage::Obtain();
}

// ################################### DESCRIPTOR STRUCTS ###################################

// WebGPUInstanceDescriptor
WebGPUInstanceDescriptor WebGPUInstanceDescriptor::Obtain() {
    WebGPUInstanceDescriptor obj;
    return obj;
}

void WebGPUInstanceDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != nullptr ? chainedStruct->Get() : nullptr;
}

//WebGPUInstanceCapabilities WebGPUInstanceDescriptor::GetFeatures() { // TODO dawn have different code
//    #ifdef __EMSCRIPTEN__
//        WebGPUInstanceCapabilities temp;
//        temp.Set(&Get().capabilities);
//    #else
//        WebGPUInstanceCapabilities temp;
//        temp.Set(&Get().features);
//    #endif
//    return temp;
//}

// WebGPURenderBundleDescriptor
WebGPURenderBundleDescriptor WebGPURenderBundleDescriptor::Obtain() {
    WebGPURenderBundleDescriptor obj;
    return obj;
}

void WebGPURenderBundleDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != nullptr ? chainedStruct->Get() : nullptr;
}

void WebGPURenderBundleDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// WebGPURenderBundleEncoderDescriptor
WebGPURenderBundleEncoderDescriptor WebGPURenderBundleEncoderDescriptor::Obtain() {
    WebGPURenderBundleEncoderDescriptor obj;
    return obj;
}

void WebGPURenderBundleEncoderDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPURenderBundleEncoderDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPURenderBundleEncoderDescriptor::SetColorFormats(WGPUVectorTextureFormat* colorFormats) {
    if(colorFormats != NULL) {
        Get().colorFormatCount = colorFormats->size();
        Get().colorFormats = reinterpret_cast<const WGPUTextureFormat*>(colorFormats->data());
    }
    else {
        Get().colorFormatCount = 0;
        Get().colorFormats = NULL;
    }
}

void WebGPURenderBundleEncoderDescriptor::SetDepthStencilFormat(WGPUTextureFormat depthStencilFormat) {
    Get().depthStencilFormat = depthStencilFormat;
}

void WebGPURenderBundleEncoderDescriptor::SetSampleCount(int sampleCount) {
    Get().sampleCount = sampleCount;
}

void WebGPURenderBundleEncoderDescriptor::SetDepthReadOnly(int depthReadOnly) {
    Get().depthReadOnly = depthReadOnly;
}

void WebGPURenderBundleEncoderDescriptor::SetStencilReadOnly(int stencilReadOnly) {
    Get().stencilReadOnly = stencilReadOnly;
}

// WebGPUQuerySetDescriptor
WebGPUQuerySetDescriptor WebGPUQuerySetDescriptor::Obtain() {
    WebGPUQuerySetDescriptor obj;
    return obj;
}

void WebGPUQuerySetDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUQuerySetDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUQuerySetDescriptor::SetType(WGPUQueryType type) {
    Get().type = type;
}

void WebGPUQuerySetDescriptor::SetCount(int count) {
    Get().count = count;
}

// WebGPUSamplerDescriptor
WebGPUSamplerDescriptor WebGPUSamplerDescriptor::Obtain() {
    WebGPUSamplerDescriptor obj;
    return obj;
}

void WebGPUSamplerDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUSamplerDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUSamplerDescriptor::SetAddressModeU(WGPUAddressMode addressModeU) {
    Get().addressModeU = addressModeU;
}

void WebGPUSamplerDescriptor::SetAddressModeV(WGPUAddressMode addressModeV) {
    Get().addressModeV = addressModeV;
}

void WebGPUSamplerDescriptor::SetAddressModeW(WGPUAddressMode addressModeW) {
    Get().addressModeW = addressModeW;
}

void WebGPUSamplerDescriptor::SetMagFilter(WGPUFilterMode magFilter) {
    Get().magFilter = magFilter;
}

void WebGPUSamplerDescriptor::SetMinFilter(WGPUFilterMode minFilter) {
    Get().minFilter = minFilter;
}

void WebGPUSamplerDescriptor::SetMipmapFilter(WGPUMipmapFilterMode mipmapFilter) {
    Get().mipmapFilter = mipmapFilter;
}

void WebGPUSamplerDescriptor::SetLodMinClamp(float lodMinClamp) {
    Get().lodMinClamp = lodMinClamp;
}

void WebGPUSamplerDescriptor::SetLodMaxClamp(float lodMaxClamp) {
    Get().lodMaxClamp = lodMaxClamp;
}

void WebGPUSamplerDescriptor::SetCompare(WGPUCompareFunction compare) {
    Get().compare = compare;
}

void WebGPUSamplerDescriptor::SetMaxAnisotropy(int maxAnisotropy) {
    Get().maxAnisotropy = maxAnisotropy;
}

// WebGPUTextureViewDescriptor
WebGPUTextureViewDescriptor WebGPUTextureViewDescriptor::Obtain() {
    WebGPUTextureViewDescriptor obj;
    return obj;
}

void WebGPUTextureViewDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUTextureViewDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUTextureViewDescriptor::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void WebGPUTextureViewDescriptor::SetDimension(WGPUTextureViewDimension dimension) {
    Get().dimension = dimension;
}

void WebGPUTextureViewDescriptor::SetBaseMipLevel(int baseMipLevel) {
    Get().baseMipLevel = baseMipLevel;
}

void WebGPUTextureViewDescriptor::SetMipLevelCount(int mipLevelCount) {
    Get().mipLevelCount = mipLevelCount;
}

void WebGPUTextureViewDescriptor::SetBaseArrayLayer(int baseArrayLayer) {
    Get().baseArrayLayer = baseArrayLayer;
}

void WebGPUTextureViewDescriptor::SetArrayLayerCount(int arrayLayerCount) {
    Get().arrayLayerCount = arrayLayerCount;
}

void WebGPUTextureViewDescriptor::SetAspect(WGPUTextureAspect aspect) {
    Get().aspect = aspect;
}

void WebGPUTextureViewDescriptor::SetUsage(WGPUTextureUsage usage) {
    Get().usage = usage;
}

// WebGPUComputePassDescriptor
WebGPUComputePassDescriptor WebGPUComputePassDescriptor::Obtain() {
    WebGPUComputePassDescriptor obj;
    return obj;
}

void WebGPUComputePassDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUComputePassDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUComputePassDescriptor::SetTimestampWrites(WebGPUComputePassTimestampWrites* timestampWrites) {
    Get().timestampWrites = timestampWrites != NULL ? &timestampWrites->Get() : NULL;
}

// WebGPUTextureDescriptor
WebGPUTextureDescriptor WebGPUTextureDescriptor::Obtain() {
    WebGPUTextureDescriptor obj;
    return obj;
}

void WebGPUTextureDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUTextureDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUTextureDescriptor::SetUsage(WGPUTextureUsage usage) {
    Get().usage = usage;
}

void WebGPUTextureDescriptor::SetDimension(WGPUTextureDimension dimension) {
    Get().dimension = dimension;
}

WebGPUExtent3D WebGPUTextureDescriptor::GetSize() {
    WebGPUExtent3D temp(&Get().size);
    return temp;
}

void WebGPUTextureDescriptor::SetFormat(WGPUTextureFormat format) {
    Get().format = format;
}

void WebGPUTextureDescriptor::SetMipLevelCount(int mipLevelCount) {
    Get().mipLevelCount = mipLevelCount;
}

void WebGPUTextureDescriptor::SetSampleCount(int sampleCount) {
    Get().sampleCount = sampleCount;
}

void WebGPUTextureDescriptor::SetViewFormats(WGPUVectorTextureFormat* viewFormats) {
    if(viewFormats != NULL) {
        Get().viewFormatCount = viewFormats->size();
        Get().viewFormats = reinterpret_cast<const WGPUTextureFormat*>(viewFormats->data());
    }
    else {
        Get().viewFormatCount = 0;
        Get().viewFormats = NULL;
    }
}

// WebGPURenderPassDescriptor
WebGPURenderPassDescriptor WebGPURenderPassDescriptor::Obtain() {
    WebGPURenderPassDescriptor obj;
    return obj;
}

void WebGPURenderPassDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPURenderPassDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPURenderPassDescriptor::SetColorAttachments(WGPUVectorRenderPassColorAttachment* values) {
    if(values != NULL) {
        Get().colorAttachmentCount = values->size();
        Get().colorAttachments = reinterpret_cast<const WGPURenderPassColorAttachment*>(values->data());
    }
    else {
        Get().colorAttachmentCount = 0;
        Get().colorAttachments = NULL;
    }
}

void WebGPURenderPassDescriptor::SetDepthStencilAttachment(WebGPURenderPassDepthStencilAttachment* attachment) {
    Get().depthStencilAttachment = attachment == NULL ? NULL : &(attachment->Get());
}

void WebGPURenderPassDescriptor::SetOcclusionQuerySet(WebGPUQuerySet* timestampWrites) {
    Get().occlusionQuerySet = timestampWrites == NULL ? NULL : timestampWrites->Get();
}

void WebGPURenderPassDescriptor::SetTimestampWrites(WebGPURenderPassTimestampWrites* timestampWrites) {
    Get().timestampWrites = timestampWrites == NULL ? NULL : &(timestampWrites->Get());
}

// WebGPUCommandEncoderDescriptor
WebGPUCommandEncoderDescriptor WebGPUCommandEncoderDescriptor::Obtain() {
    WebGPUCommandEncoderDescriptor obj;
    return obj;
}

void WebGPUCommandEncoderDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUCommandEncoderDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// WebGPUCommandBufferDescriptor
WebGPUCommandBufferDescriptor WebGPUCommandBufferDescriptor::Obtain() {
    WebGPUCommandBufferDescriptor obj;
    return obj;
}

void WebGPUCommandBufferDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUCommandBufferDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// WebGPURenderPipelineDescriptor
WebGPURenderPipelineDescriptor WebGPURenderPipelineDescriptor::Obtain() {
    WebGPURenderPipelineDescriptor obj;
    return obj;
}

void WebGPURenderPipelineDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPURenderPipelineDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

WebGPUVertexState WebGPURenderPipelineDescriptor::GetVertex() {
    WebGPUVertexState temp;
    temp.Set(&Get().vertex);
    return temp;
}

WebGPUPrimitiveState WebGPURenderPipelineDescriptor::GetPrimitive() {
    WebGPUPrimitiveState temp;
    temp.Set(&Get().primitive);
    return temp;
}

void WebGPURenderPipelineDescriptor::SetFragment(WebGPUFragmentState* fragment) {
    Get().fragment = &fragment->Get();
}

void WebGPURenderPipelineDescriptor::SetDepthStencil(WebGPUDepthStencilState* depthStencilState) {
    Get().depthStencil = depthStencilState != NULL ? &depthStencilState->Get() : NULL;
}

WebGPUMultisampleState WebGPURenderPipelineDescriptor::GetMultisample() {
    WebGPUMultisampleState temp;
    temp.Set(&Get().multisample);
    return temp;
}

void WebGPURenderPipelineDescriptor::SetLayout(WebGPUPipelineLayout* pipelineLayout) {
    Get().layout = pipelineLayout != NULL ? pipelineLayout->Get() : NULL;
}

// WebGPUShaderModuleDescriptor
WebGPUShaderModuleDescriptor WebGPUShaderModuleDescriptor::Obtain() {
    WebGPUShaderModuleDescriptor obj;
    return obj;
}

void WebGPUShaderModuleDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUShaderModuleDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

// WebGPUBindGroupLayoutDescriptor
WebGPUBindGroupLayoutDescriptor WebGPUBindGroupLayoutDescriptor::Obtain() {
    WebGPUBindGroupLayoutDescriptor obj;
    return obj;
}

void WebGPUBindGroupLayoutDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUBindGroupLayoutDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUBindGroupLayoutDescriptor::SetEntries(WGPUVectorBindGroupLayoutEntry* entries) {
    if(entries != NULL) {
        Get().entryCount = entries->size();
        Get().entries = reinterpret_cast<const WGPUBindGroupLayoutEntry*>(entries->data());
    }
    else {
        Get().entryCount = 0;
        Get().entries = NULL;
    }
}

// WebGPUProgrammableStageDescriptor
WebGPUProgrammableStageDescriptor WebGPUProgrammableStageDescriptor::Obtain() {
    WebGPUProgrammableStageDescriptor obj;
    return obj;
}

void WebGPUProgrammableStageDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}




// WebGPUComputePipelineDescriptor
WebGPUComputePipelineDescriptor WebGPUComputePipelineDescriptor::Obtain() {
    WebGPUComputePipelineDescriptor obj;
    return obj;
}

void WebGPUComputePipelineDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUComputePipelineDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

WebGPUProgrammableStageDescriptor WebGPUComputePipelineDescriptor::GetCompute() {
    WebGPUProgrammableStageDescriptor temp;
    temp.Set(&Get().compute);
    return temp;
}

// WebGPUQueueDescriptor
WebGPUQueueDescriptor WebGPUQueueDescriptor::Obtain() {
    WebGPUQueueDescriptor obj;
    return obj;
}

void WebGPUQueueDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get()->nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUQueueDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get()->label = stringView.Get();
}

// WebGPUBufferDescriptor
WebGPUBufferDescriptor WebGPUBufferDescriptor::Obtain() {
    WebGPUBufferDescriptor obj;
    return obj;
}

void WebGPUBufferDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUBufferDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUBufferDescriptor::SetUsage(WGPUBufferUsage usage) {
    Get().usage = usage;
}

void WebGPUBufferDescriptor::SetSize(int size) {
    Get().size = size;
}

void WebGPUBufferDescriptor::SetMappedAtCreation(bool mappedAtCreation) {
    Get().mappedAtCreation = mappedAtCreation;
}

// WebGPUBindGroupDescriptor
WebGPUBindGroupDescriptor WebGPUBindGroupDescriptor::Obtain() {
    WebGPUBindGroupDescriptor obj;
    return obj;
}

void WebGPUBindGroupDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUBindGroupDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUBindGroupDescriptor::SetLayout(WebGPUBindGroupLayout* layout) {
    Get().layout = layout->Get();
}

void WebGPUBindGroupDescriptor::SetEntries(WGPUVectorBindGroupEntry* entries) {
    if(entries != NULL) {
        Get().entryCount = entries->size();
        Get().entries = reinterpret_cast<const WGPUBindGroupEntry*>(entries->data());
    }
    else {
        Get().entryCount = 0;
        Get().entries = NULL;
    }
}

// WebGPUPipelineLayoutDescriptor
WebGPUPipelineLayoutDescriptor WebGPUPipelineLayoutDescriptor::Obtain() {
    WebGPUPipelineLayoutDescriptor obj;
    return obj;
}

void WebGPUPipelineLayoutDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUPipelineLayoutDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUPipelineLayoutDescriptor::SetBindGroupLayouts(WGPUVectorBindGroupLayout* bindGroupLayouts) {
    if(bindGroupLayouts != NULL) {
        Get().bindGroupLayoutCount = bindGroupLayouts->size();
        Get().bindGroupLayouts = reinterpret_cast<const WGPUBindGroupLayout*>(bindGroupLayouts->data());
    }
    else {
        Get().bindGroupLayoutCount = 0;
        Get().bindGroupLayouts = NULL;
    }
}

// WebGPUDeviceDescriptor
WebGPUDeviceDescriptor WebGPUDeviceDescriptor::Obtain() {
    WebGPUDeviceDescriptor obj;
    return obj;
}

void WebGPUDeviceDescriptor::SetNextInChain(WebGPUChainedStruct* chainedStruct) {
    Get().nextInChain = chainedStruct != NULL ? chainedStruct->Get() : NULL;
}

void WebGPUDeviceDescriptor::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    Get().label = stringView.Get();
}

void WebGPUDeviceDescriptor::SetRequiredLimits(WebGPULimits* limits) {
    Get().requiredLimits = &(limits->Get());
}

void WebGPUDeviceDescriptor::SetRequiredFeatures(WGPUVectorFeatureName* features) {
    if(features != NULL) {
        Get().requiredFeatureCount = features->size();
        Get().requiredFeatures = features->data();
    }
    else {
        Get().requiredFeatureCount = 0;
        Get().requiredFeatures = NULL;
    }
}

WebGPUQueueDescriptor WebGPUDeviceDescriptor::GetDefaultQueue() {
    WebGPUQueueDescriptor temp;
    temp.Set(&Get().defaultQueue);
    return temp;
}

// ################################### OPAQUE POINTER ###################################

// WebGPUSampler
void WebGPUSampler::AddRefInternal() {
    wgpuSamplerAddRef(Get());
}

void WebGPUSampler::ReleaseInternal() {
    wgpuSamplerRelease(Get());
}

void WebGPUSampler::SetLabel(const char* label) {
    WebGPUStringView stringView(label);
    wgpuSamplerSetLabel(Get(), stringView.Get());
}

// WebGPURenderBundleEncoder
WebGPURenderBundleEncoder WebGPURenderBundleEncoder::Obtain() {
    WebGPURenderBundleEncoder obj;
    return obj;
}

void WebGPURenderBundleEncoder::AddRefInternal() {
    wgpuRenderBundleEncoderAddRef(Get());
}

void WebGPURenderBundleEncoder::ReleaseInternal() {
    wgpuRenderBundleEncoderRelease(Get());
}

void WebGPURenderBundleEncoder::SetPipeline(WebGPURenderPipeline* renderPipeline) {
    wgpuRenderBundleEncoderSetPipeline(Get(), renderPipeline->Get());
}

void WebGPURenderBundleEncoder::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
    wgpuRenderBundleEncoderDraw(Get(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void WebGPURenderBundleEncoder::DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance) {
    wgpuRenderBundleEncoderDrawIndexed(Get(), indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void WebGPURenderBundleEncoder::DrawIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderBundleEncoderDrawIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void WebGPURenderBundleEncoder::DrawIndexedIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderBundleEncoderDrawIndexedIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void WebGPURenderBundleEncoder::SetBindGroup(int groupIndex, WebGPUBindGroup* group, WGPUVectorInt* offsets) {
    int dynamicOffsetCount = 0;
    uint32_t* dynamicOffsets = NULL;
    if(offsets != NULL) {
        dynamicOffsetCount = offsets->size();
        dynamicOffsets = (uint32_t*)offsets->data();
    }
    wgpuRenderBundleEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, dynamicOffsets);
}

void WebGPURenderBundleEncoder::SetVertexBuffer(int slot, WebGPUBuffer* buffer, int offset, int size) {
    wgpuRenderBundleEncoderSetVertexBuffer(Get(), slot, buffer->Get(), offset, size);
}

void WebGPURenderBundleEncoder::SetIndexBuffer(WebGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size) {
    wgpuRenderBundleEncoderSetIndexBuffer(Get(), buffer->Get(), format, offset, size);
}

void WebGPURenderBundleEncoder::InsertDebugMarker(const char* label) {
    WebGPUStringView stringView(label);
    wgpuRenderBundleEncoderInsertDebugMarker(Get(), stringView.Get());
}

void WebGPURenderBundleEncoder::PopDebugGroup() {
    wgpuRenderBundleEncoderPopDebugGroup(Get());
}

void WebGPURenderBundleEncoder::PushDebugGroup(const char* label) {
    WebGPUStringView stringView(label);
    wgpuRenderBundleEncoderPushDebugGroup(Get(), stringView.Get());
}

void WebGPURenderBundleEncoder::SetLabel(const char* label) {
    WebGPUStringView stringView(label);
    wgpuRenderBundleEncoderSetLabel(Get(), stringView.Get());
}

void WebGPURenderBundleEncoder::Finish(WebGPURenderBundleDescriptor* descriptor, WebGPURenderBundle* bundle) {
    bundle->Set(wgpuRenderBundleEncoderFinish(Get(), descriptor != NULL ? &descriptor->Get() : NULL));
}

// WebGPUTextureView
WebGPUTextureView WebGPUTextureView::Obtain() {
    WebGPUTextureView obj;
    return obj;
}

void WebGPUTextureView::AddRefInternal() {
    wgpuTextureViewAddRef(Get());
}

void WebGPUTextureView::ReleaseInternal() {
    wgpuTextureViewRelease(Get());
}

// WebGPUTexture
WebGPUTexture WebGPUTexture::Obtain() {
    WebGPUTexture obj;
    return obj;
}

void WebGPUTexture::AddRefInternal() {
    wgpuTextureAddRef(Get());
}

void WebGPUTexture::ReleaseInternal() {
    wgpuTextureRelease(Get());
}

void WebGPUTexture::CreateView(WebGPUTextureViewDescriptor* textureViewDescriptor, WebGPUTextureView* textureView) {
    textureView->Set(wgpuTextureCreateView(Get(), &(textureViewDescriptor->Get())));
}

WGPUTextureFormat WebGPUTexture::GetFormat() {
    return wgpuTextureGetFormat(Get());
}


void WebGPUTexture::Destroy() {
    wgpuTextureDestroy(Get());
}

// WebGPUShaderModule
WebGPUShaderModule WebGPUShaderModule::Obtain() {
    WebGPUShaderModule obj;
    return obj;
}

void WebGPUShaderModule::AddRefInternal() {
    wgpuShaderModuleAddRef(Get());
}

void WebGPUShaderModule::ReleaseInternal() {
    wgpuShaderModuleRelease(Get());
}

// WebGPURenderPipeline
WebGPURenderPipeline WebGPURenderPipeline::Obtain() {
    WebGPURenderPipeline obj;
    return obj;
}

void WebGPURenderPipeline::AddRefInternal() {
    wgpuRenderPipelineAddRef(Get());
}

void WebGPURenderPipeline::ReleaseInternal() {
    wgpuRenderPipelineRelease(Get());
}

// WebGPURenderPassEncoder
WebGPURenderPassEncoder WebGPURenderPassEncoder::Obtain() {
    WebGPURenderPassEncoder obj;
    return obj;
}

void WebGPURenderPassEncoder::AddRefInternal() {
    wgpuRenderPassEncoderAddRef(Get());
}

void WebGPURenderPassEncoder::ReleaseInternal() {
    wgpuRenderPassEncoderRelease(Get());
}

void WebGPURenderPassEncoder::End() {
    wgpuRenderPassEncoderEnd(Get());
}

void WebGPURenderPassEncoder::SetPipeline(WebGPURenderPipeline* renderPipeline) {
    wgpuRenderPassEncoderSetPipeline(Get(), renderPipeline->Get());
}

void WebGPURenderPassEncoder::BeginOcclusionQuery(int queryIndex) {
    wgpuRenderPassEncoderBeginOcclusionQuery(Get(), queryIndex);
}

void WebGPURenderPassEncoder::Draw(int vertexCount, int instanceCount, int firstVertex, int firstInstance) {
    wgpuRenderPassEncoderDraw(Get(), vertexCount, instanceCount, firstVertex, firstInstance);
}

void WebGPURenderPassEncoder::DrawIndexed(int indexCount, int instanceCount, int firstIndex, int baseVertex, int firstInstance) {
    wgpuRenderPassEncoderDrawIndexed(Get(), indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void WebGPURenderPassEncoder::DrawIndexedIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderPassEncoderDrawIndexedIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void WebGPURenderPassEncoder::DrawIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuRenderPassEncoderDrawIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void WebGPURenderPassEncoder::EndOcclusionQuery() {
    wgpuRenderPassEncoderEndOcclusionQuery(Get());
}

void WebGPURenderPassEncoder::ExecuteBundles(WGPUVectorRenderBundle* values) {
    if(values != NULL) {
        int size = values->size();
        wgpuRenderPassEncoderExecuteBundles(Get(), size, reinterpret_cast<WGPURenderBundle const * >(values->data()));
    }
    else {
        wgpuRenderPassEncoderExecuteBundles(Get(), 0, NULL);
    }
}

void WebGPURenderPassEncoder::InsertDebugMarker(const char* label) {
    WebGPUStringView stringView(label);
    wgpuRenderPassEncoderInsertDebugMarker(Get(), stringView.Get());
}

void WebGPURenderPassEncoder::PopDebugGroup() {
    wgpuRenderPassEncoderPopDebugGroup(Get());
}

void WebGPURenderPassEncoder::PushDebugGroup(const char* label) {
    WebGPUStringView stringView(label);
    wgpuRenderPassEncoderPushDebugGroup(Get(), stringView.Get());
}

void WebGPURenderPassEncoder::SetBindGroup(int groupIndex, WebGPUBindGroup* group, WGPUVectorInt* offsets) {
    int dynamicOffsetCount = 0;
    uint32_t* dynamicOffsets = NULL;
    if(offsets != NULL) {
        dynamicOffsetCount = offsets->size();
        dynamicOffsets = (uint32_t*)offsets->data();
    }
    wgpuRenderPassEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, dynamicOffsets);
}

void WebGPURenderPassEncoder::SetBlendConstant(WebGPUColor* color) {
    wgpuRenderPassEncoderSetBlendConstant(Get(), color != NULL ? color->Get() : NULL);
}

void WebGPURenderPassEncoder::SetIndexBuffer(WebGPUBuffer* buffer, WGPUIndexFormat format, int offset, int size) {
    wgpuRenderPassEncoderSetIndexBuffer(Get(), buffer->Get(), format, offset, size);
}

void WebGPURenderPassEncoder::SetLabel(const char* label) {
    WebGPUStringView stringView(label);
    wgpuRenderPassEncoderSetLabel(Get(), stringView.Get());
}

void WebGPURenderPassEncoder::SetScissorRect(int x, int y, int width, int height) {
    wgpuRenderPassEncoderSetScissorRect(Get(), x, y, width, height);
}

void WebGPURenderPassEncoder::SetStencilReference(int reference) {
    wgpuRenderPassEncoderSetStencilReference(Get(), reference);
}

void WebGPURenderPassEncoder::SetVertexBuffer(int slot, WebGPUBuffer* buffer, int offset, int size) {
    wgpuRenderPassEncoderSetVertexBuffer(Get(), slot, buffer->Get(), offset, size);
}

void WebGPURenderPassEncoder::SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) {
    wgpuRenderPassEncoderSetViewport(Get(), x, y, width, height, minDepth, maxDepth);
}

// WebGPUQuerySet
void WebGPUQuerySet::AddRefInternal() {
    wgpuQuerySetAddRef(Get());
}

void WebGPUQuerySet::ReleaseInternal() {
    wgpuQuerySetRelease(Get());
}

void WebGPUQuerySet::Destroy() {
    wgpuQuerySetDestroy(Get());
}

int WebGPUQuerySet::GetCount() {
    return wgpuQuerySetGetCount(Get());
}

WGPUQueryType WebGPUQuerySet::GetType() {
    return wgpuQuerySetGetType(Get());
}

void WebGPUQuerySet::SetLabel(const char* label) {
    WebGPUStringView stringView(label);
    wgpuQuerySetSetLabel(Get(), stringView.Get());
}

// WebGPUPipelineLayout
void WebGPUPipelineLayout::AddRefInternal() {
    wgpuPipelineLayoutAddRef(Get());
}

void WebGPUPipelineLayout::ReleaseInternal() {
    wgpuPipelineLayoutRelease(Get());
}

void WebGPUPipelineLayout::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    wgpuPipelineLayoutSetLabel(Get(), stringView.Get());
}

// WebGPUInstance
void WebGPUInstance::AddRefInternal() {
    wgpuInstanceAddRef(Get());
}

void WebGPUInstance::ReleaseInternal() {
    wgpuInstanceRelease(Get());
}

bool WebGPUInstance::IsValid() {
    return Get() ? true : false;
}

void WebGPUInstance::RequestAdapter(WebGPURequestAdapterOptions* options, WGPUCallbackMode mode, RequestAdapterCallback* callback) {
    WGPURequestAdapterCallbackInfo callbackInfo = {};
    callbackInfo.mode = mode;
    callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter ad, WGPUStringView message, void* callback_param, void*) {
        RequestAdapterCallback* cback = reinterpret_cast<RequestAdapterCallback*>(callback_param);
        WebGPUAdapter* adapter = new WebGPUAdapter();
        adapter->Set(ad);
        cback->OnCallback(status, adapter);
    };
    callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
    callbackInfo.userdata2 = NULL;
    auto result = wgpuInstanceRequestAdapter(Get(), &(options->Get()), callbackInfo);
}

WebGPUSurface* WebGPUInstance::CreateWebSurface(const char* canvas) {
    #ifdef __EMSCRIPTEN__
        WGPUEmscriptenSurfaceSourceCanvasHTMLSelector canvasDesc {};
        WebGPUStringView stringView(canvas);
        canvasDesc.chain.sType = WGPUSType_EmscriptenSurfaceSourceCanvasHTMLSelector;
        canvasDesc.selector = stringView.Get();
        WGPUSurfaceDescriptor surfDesc{};
        surfDesc.nextInChain = (WGPUChainedStruct*)&canvasDesc;
        WebGPUSurface* surface = new WebGPUSurface();
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfDesc));
    #else
        WebGPUSurface* surface = NULL;
    #endif
    return surface;
}

WebGPUSurface* WebGPUInstance::CreateWindowsSurface(void * hwnd) {
    WebGPUSurface* surface = NULL;
    #if _WIN32
        surface = new WebGPUSurface();
        HINSTANCE hinstance = GetModuleHandle(NULL);
        WGPUSurfaceSourceWindowsHWND fromWindowsHWND;
        fromWindowsHWND.chain.next = NULL;
        fromWindowsHWND.chain.sType = WGPUSType_SurfaceSourceWindowsHWND;
        fromWindowsHWND.hinstance = hinstance;
        fromWindowsHWND.hwnd = hwnd;
        WGPUSurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.nextInChain = &fromWindowsHWND.chain;
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
    #endif
    return surface;
}

WebGPUSurface* WebGPUInstance::CreateLinuxSurface(bool isWayland, void * windowOrSurface, void* display) {
    WebGPUSurface* surface = NULL;
    #if __linux__
        surface = new WebGPUSurface();
        if(isWayland) {
            WGPUSurfaceSourceWaylandSurface fromLinux;
            fromLinux.chain.next = NULL;
            fromLinux.chain.sType = WGPUSType_SurfaceSourceWaylandSurface;
            fromLinux.display = display;
            fromLinux.surface = windowOrSurface;
            WGPUSurfaceDescriptor surfaceDescriptor{};
            surfaceDescriptor.nextInChain = &fromLinux.chain;
            surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
        }
        else {
            WGPUSurfaceSourceXlibWindow fromLinux;
            fromLinux.chain.next = NULL;
            fromLinux.chain.sType = WGPUSType_SurfaceSourceXlibWindow;
            fromLinux.display = display;
            fromLinux.window = (uint64_t)windowOrSurface;
            WGPUSurfaceDescriptor surfaceDescriptor{};
            surfaceDescriptor.nextInChain = &fromLinux.chain;
            surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
        }
    #endif
    return surface;
}

WebGPUSurface* WebGPUInstance::CreateMacSurface(void * windowHandle) {
    WebGPUSurface* surface = NULL;
    #if TARGET_OS_MAC
        surface = new WebGPUSurface();
        id metal_layer = [CAMetalLayer layer];

        NSWindow* ns_window = (NSWindow*)windowHandle;
        [ns_window.contentView setWantsLayer : YES];
        [ns_window.contentView setLayer : metal_layer];

        WGPUSurfaceSourceMetalLayer fromMac;
        fromMac.chain.next = NULL;
        fromMac.chain.sType = WGPUSType_SurfaceSourceMetalLayer;
        fromMac.layer = metal_layer;
        WGPUSurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.nextInChain = &fromMac.chain;
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
    #endif
    return surface;
}

WebGPUSurface* WebGPUInstance::CreateAndroidSurface(WGPUAndroidWindow* window) {
    WebGPUSurface* surface = NULL;
    #if __ANDROID__
        void* androidWindow = window->GetWindow();
        surface = new WebGPUSurface();
        WGPUSurfaceSourceAndroidNativeWindow androidSurfaceWindow;
        androidSurfaceWindow.chain.next = NULL;
        androidSurfaceWindow.chain.sType = WGPUSType_SurfaceSourceAndroidNativeWindow;
        androidSurfaceWindow.window = androidWindow;
        WGPUSurfaceDescriptor surfaceDescriptor{};
        surfaceDescriptor.nextInChain = &androidSurfaceWindow.chain;
        surface->Set(wgpuInstanceCreateSurface(Get(), &surfaceDescriptor));
    #endif
    return surface;
}

void WebGPUInstance::ProcessEvents() {
    #ifdef __EMSCRIPTEN__
        wgpuInstanceProcessEvents(Get());
//        emscripten_sleep(100);
    #else // __EMSCRIPTEN__
        wgpuInstanceProcessEvents(Get());
    #endif
}

WGPUWaitStatus WebGPUInstance::WaitAny(WGPUVectorFutureWaitInfo* futureVector, int timeoutNS) {
    int futureCount = 0;
    WGPUFutureWaitInfo* futures = NULL;
    if(futureVector != NULL) {
        futureCount = futureVector->size();
        futures = (WGPUFutureWaitInfo*)futureVector->data();
    }
    return wgpuInstanceWaitAny(Get(), futureCount, futures, timeoutNS);
}

// WebGPUDevice
void WebGPUDevice::AddRefInternal() {
    wgpuDeviceAddRef(Get());
}

void WebGPUDevice::ReleaseInternal() {
    wgpuDeviceRelease(Get());
}

void WebGPUDevice::CreateBindGroup(WebGPUBindGroupDescriptor* descriptor, WebGPUBindGroup* valueOut) {
    valueOut->Set(wgpuDeviceCreateBindGroup(Get(), &descriptor->Get()));
}

void WebGPUDevice::CreateBindGroupLayout(WebGPUBindGroupLayoutDescriptor* descriptor, WebGPUBindGroupLayout* valueOut) {
    valueOut->Set(wgpuDeviceCreateBindGroupLayout(Get(), &descriptor->Get()));
}

void WebGPUDevice::CreateBuffer(WebGPUBufferDescriptor* descriptor, WebGPUBuffer* valueOut) {
    valueOut->Set(wgpuDeviceCreateBuffer(Get(), &descriptor->Get()));
}

void WebGPUDevice::CreateCommandEncoder(WebGPUCommandEncoderDescriptor* descriptor, WebGPUCommandEncoder* valueOut) {
    valueOut->Set(wgpuDeviceCreateCommandEncoder(Get(), descriptor != NULL ? &descriptor->Get() : NULL));
}

void WebGPUDevice::CreateComputePipeline(WebGPUComputePipelineDescriptor* descriptor, WebGPUComputePipeline* valueOut) {
    valueOut->Set(wgpuDeviceCreateComputePipeline(Get(), &(descriptor->Get())));
}

void WebGPUDevice::CreatePipelineLayout(WebGPUPipelineLayoutDescriptor* descriptor, WebGPUPipelineLayout* valueOut) {
    valueOut->Set(wgpuDeviceCreatePipelineLayout(Get(), &(descriptor->Get())));
}

void WebGPUDevice::CreateQuerySet(WebGPUQuerySetDescriptor* descriptor, WebGPUQuerySet* valueOut) {
    valueOut->Set(wgpuDeviceCreateQuerySet(Get(), &(descriptor->Get())));
}

void WebGPUDevice::CreateRenderBundleEncoder(WebGPURenderBundleEncoderDescriptor* descriptor, WebGPURenderBundleEncoder* valueOut) {
    valueOut->Set(wgpuDeviceCreateRenderBundleEncoder(Get(), &(descriptor->Get())));
}

void WebGPUDevice::CreateRenderPipeline(WebGPURenderPipelineDescriptor* descriptor, WebGPURenderPipeline* valueOut) {
    valueOut->Set(wgpuDeviceCreateRenderPipeline(Get(), &descriptor->Get()));
}

void WebGPUDevice::CreateSampler(WebGPUSamplerDescriptor* descriptor, WebGPUSampler* valueOut) {
    valueOut->Set(wgpuDeviceCreateSampler(Get(), &descriptor->Get()));
}

void WebGPUDevice::CreateShaderModule(WebGPUShaderModuleDescriptor* descriptor, WebGPUShaderModule* valueOut) {
    valueOut->Set(wgpuDeviceCreateShaderModule(Get(), &descriptor->Get()));
}

void WebGPUDevice::CreateTexture(WebGPUTextureDescriptor* descriptor, WebGPUTexture* valueOut) {
    valueOut->Set(wgpuDeviceCreateTexture(Get(), &descriptor->Get()));
}

void WebGPUDevice::Destroy() {
    wgpuDeviceDestroy(Get());
}

void WebGPUDevice::GetFeatures(WebGPUSupportedFeatures* features) {
    wgpuDeviceGetFeatures(Get(), &features->Get());
}

bool WebGPUDevice::HasFeature(WGPUFeatureName feature) {
    return wgpuDeviceHasFeature(Get(), feature);
}

void WebGPUDevice::GetLimits(WebGPULimits* limits) {
    wgpuDeviceGetLimits(Get(), &limits->Get());
}

WebGPUQueue WebGPUDevice::GetQueue() {
    WebGPUQueue temp;
    temp.Set(wgpuDeviceGetQueue(Get()));
    return temp;
}

// WebGPUComputePassEncoder
WebGPUComputePassEncoder WebGPUComputePassEncoder::Obtain() {
    WebGPUComputePassEncoder obj;
    return obj;
}

void WebGPUComputePassEncoder::AddRefInternal() {
    wgpuComputePassEncoderAddRef(Get());
}

void WebGPUComputePassEncoder::ReleaseInternal() {
    wgpuComputePassEncoderRelease(Get());
}

void WebGPUComputePassEncoder::SetDispatchWorkgroups(int workgroupCountX, int workgroupCountY, int workgroupCountZ) {
    wgpuComputePassEncoderDispatchWorkgroups(Get(), workgroupCountX, workgroupCountY, workgroupCountZ);
}

void WebGPUComputePassEncoder::DispatchWorkgroupsIndirect(WebGPUBuffer* indirectBuffer, int indirectOffset) {
    wgpuComputePassEncoderDispatchWorkgroupsIndirect(Get(), indirectBuffer->Get(), indirectOffset);
}

void WebGPUComputePassEncoder::End() {
    wgpuComputePassEncoderEnd(Get());
}

void WebGPUComputePassEncoder::InsertDebugMarker(const char* markerLabel) {
    WebGPUStringView stringView(markerLabel);
    wgpuComputePassEncoderInsertDebugMarker(Get(), stringView.Get());
}

void WebGPUComputePassEncoder::PopDebugGroup() {
    wgpuComputePassEncoderPopDebugGroup(Get());
}

void WebGPUComputePassEncoder::PushDebugGroup(const char* groupLabel) {
    WebGPUStringView stringView(groupLabel);
    wgpuComputePassEncoderPushDebugGroup(Get(), stringView.Get());
}

void WebGPUComputePassEncoder::SetBindGroup(int groupIndex, WebGPUBindGroup* group, WGPUVectorInt* offsets) {
    int dynamicOffsetCount = 0;
    uint32_t* dynamicOffsets = NULL;
    if(offsets != NULL) {
        dynamicOffsetCount = offsets->size();
        dynamicOffsets = (uint32_t*)offsets->data();
    }
    wgpuComputePassEncoderSetBindGroup(Get(), groupIndex, group->Get(), dynamicOffsetCount, dynamicOffsets);
}

void WebGPUComputePassEncoder::SetLabel(const char* label) {
    WebGPUStringView stringView(label);
    wgpuComputePassEncoderSetLabel(Get(), stringView.Get());
}

void WebGPUComputePassEncoder::SetPipeline(WebGPUComputePipeline* pipeline) {
    wgpuComputePassEncoderSetPipeline(Get(), pipeline->Get());
}

// WebGPUCommandBuffer
WebGPUCommandBuffer WebGPUCommandBuffer::Obtain() {
    WebGPUCommandBuffer obj;
    return obj;
}

void WebGPUCommandBuffer::AddRefInternal() {
    wgpuCommandBufferAddRef(Get());
}

void WebGPUCommandBuffer::ReleaseInternal() {
    wgpuCommandBufferRelease(Get());
}

// WebGPUCommandEncoder
WebGPUCommandEncoder WebGPUCommandEncoder::Obtain() {
    WebGPUCommandEncoder obj;
    return obj;
}

void WebGPUCommandEncoder::AddRefInternal() {
    wgpuCommandEncoderAddRef(Get());
}

void WebGPUCommandEncoder::ReleaseInternal() {
    wgpuCommandEncoderRelease(Get());
}

void WebGPUCommandEncoder::BeginComputePass(WebGPUComputePassDescriptor* descriptor, WebGPUComputePassEncoder* encoder) {
    encoder->Set(wgpuCommandEncoderBeginComputePass(Get(), &descriptor->Get()));
}

void WebGPUCommandEncoder::BeginRenderPass(WebGPURenderPassDescriptor* renderPassDescriptor, WebGPURenderPassEncoder* encoder) {
    encoder->Set(wgpuCommandEncoderBeginRenderPass(Get(), &(renderPassDescriptor->Get())));
}

void WebGPUCommandEncoder::ClearBuffer(WebGPUBuffer* buffer, int offset, int size) {
    wgpuCommandEncoderClearBuffer(Get(), buffer->Get(), offset, size);
}

void WebGPUCommandEncoder::CopyBufferToBuffer(WebGPUBuffer* source, int sourceOffset, WebGPUBuffer* destination, int destinationOffset, int size) {
    wgpuCommandEncoderCopyBufferToBuffer(Get(), source->Get(), sourceOffset, destination->Get(), destinationOffset, size);
}

void WebGPUCommandEncoder::CopyBufferToTexture(WebGPUTexelCopyBufferInfo* source, WebGPUTexelCopyTextureInfo* destination, WebGPUExtent3D* copySize) {
    wgpuCommandEncoderCopyBufferToTexture(Get(), &source->Get(), &destination->Get(), copySize->Get());
}

void WebGPUCommandEncoder::TextureToBuffer(WebGPUTexelCopyTextureInfo* source, WebGPUTexelCopyBufferInfo* destination, WebGPUExtent3D* copySize) {
    wgpuCommandEncoderCopyTextureToBuffer(Get(), &source->Get(), &destination->Get(), copySize->Get());
}

void WebGPUCommandEncoder::CopyTextureToTexture(WebGPUTexelCopyTextureInfo* source, WebGPUTexelCopyTextureInfo* destination, WebGPUExtent3D* copySize) {
    wgpuCommandEncoderCopyTextureToTexture(Get(), &source->Get(), &destination->Get(), copySize->Get());
}

void WebGPUCommandEncoder::Finish(WebGPUCommandBufferDescriptor* commandBufferDescriptor, WebGPUCommandBuffer* commandBuffer) {
    commandBuffer->Set(wgpuCommandEncoderFinish(Get(), commandBufferDescriptor != NULL ? &commandBufferDescriptor->Get() : NULL));
}

void WebGPUCommandEncoder::InsertDebugMarker(const char* markerLabel) {
    WebGPUStringView stringView(markerLabel);
    wgpuCommandEncoderInsertDebugMarker(Get(), stringView.Get());
}

void WebGPUCommandEncoder::PopDebugGroup() {
    wgpuCommandEncoderPopDebugGroup(Get());
}

void WebGPUCommandEncoder::PushDebugGroup(const char* groupLabel) {
    WebGPUStringView stringView(groupLabel);
    wgpuCommandEncoderPushDebugGroup(Get(), stringView.Get());
}

void WebGPUCommandEncoder::ResolveQuerySet(WebGPUQuerySet* querySet, int firstQuery, int queryCount, WebGPUBuffer* destination, int destinationOffset) {
    wgpuCommandEncoderResolveQuerySet(Get(), querySet->Get(), firstQuery, queryCount, destination->Get(), destinationOffset);
}

void WebGPUCommandEncoder::SetLabel(const char* label) {
    WebGPUStringView stringView(label);
    wgpuCommandEncoderSetLabel(Get(), stringView.Get());
}

void WebGPUCommandEncoder::WriteTimestamp(WebGPUQuerySet* querySet, int queryIndex) {
    wgpuCommandEncoderWriteTimestamp(Get(), querySet->Get(), queryIndex);
}

// WebGPUBuffer
void WebGPUBuffer::AddRefInternal() {
    wgpuBufferAddRef(Get());
}

void WebGPUBuffer::ReleaseInternal() {
    wgpuBufferRelease(Get());
}

void WebGPUBuffer::Unmap() {
    wgpuBufferUnmap(Get());
}

int WebGPUBuffer::GetSize() {
    return wgpuBufferGetSize(Get());
}

WGPUBufferUsage WebGPUBuffer::GetUsage() {
    return static_cast<WGPUBufferUsage>(wgpuBufferGetUsage(Get()));
}

WebGPUFuture WebGPUBuffer::MapAsync(WGPUMapMode mode, int offset, int size, WGPUCallbackMode callbackMode, BufferMapCallback* callback) {
    WGPUBufferMapCallbackInfo callbackInfo = {};
    callbackInfo.mode = callbackMode;

    callbackInfo.callback = [](WGPUMapAsyncStatus status, WGPUStringView message, void* callback_param, void*) {
        BufferMapCallback* cback = reinterpret_cast<BufferMapCallback*>(callback_param);
        cback->OnCallback(status, WebGPUStringView(message).GetString().c_str());
    };
    callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
    callbackInfo.userdata2 = NULL;

    WebGPUFuture temp;
    temp.Set(wgpuBufferMapAsync(Get(), mode, offset, size, callbackInfo));
    return temp;
}

WGPUByteBuffer WebGPUBuffer::GetConstMappedRange(int offset, int size) {
    // TODO need to test
    uint8_t* bufferData = (uint8_t*) wgpuBufferGetConstMappedRange(Get(), offset, size);
    WGPUByteBuffer buffer(bufferData, size);
    return buffer;
}

void WebGPUBuffer::Destroy() {
    wgpuBufferDestroy(Get());
}

// WebGPUBindGroup
void WebGPUBindGroup::AddRefInternal() {
    wgpuBindGroupAddRef(Get());
}

void WebGPUBindGroup::ReleaseInternal() {
    wgpuBindGroupRelease(Get());
}

void WebGPUBindGroup::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    wgpuBindGroupSetLabel(Get(), stringView.Get());
}

// WebGPUBindGroupLayout
void WebGPUBindGroupLayout::AddRefInternal() {
    wgpuBindGroupLayoutAddRef(Get());
}

void WebGPUBindGroupLayout::ReleaseInternal() {
    wgpuBindGroupLayoutRelease(Get());
}

void WebGPUBindGroupLayout::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    wgpuBindGroupLayoutSetLabel(Get(), stringView.Get());
}

// WebGPUComputePipeline
void WebGPUComputePipeline::AddRefInternal() {
    wgpuComputePipelineAddRef(Get());
}

void WebGPUComputePipeline::ReleaseInternal() {
    wgpuComputePipelineRelease(Get());
}

void WebGPUComputePipeline::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    wgpuComputePipelineSetLabel(Get(), stringView.Get());
}

WebGPUBindGroupLayout WebGPUComputePipeline::GetBindGroupLayout(int groupIndex) {
    WebGPUBindGroupLayout temp;
    temp.Set(wgpuComputePipelineGetBindGroupLayout(Get(), groupIndex));
    return temp;
}

// WebGPURenderBundle
void WebGPURenderBundle::AddRefInternal() {
    wgpuRenderBundleAddRef(Get());
}

void WebGPURenderBundle::ReleaseInternal() {
    wgpuRenderBundleRelease(Get());
}

void WebGPURenderBundle::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    wgpuRenderBundleSetLabel(Get(), stringView.Get());
}

// WebGPUAdapter
void WebGPUAdapter::AddRefInternal() {
    wgpuAdapterAddRef(Get());
}

void WebGPUAdapter::ReleaseInternal() {
    wgpuAdapterRelease(Get());
}

void WebGPUAdapter::RequestDevice(WebGPUDeviceDescriptor* descriptor, WGPUCallbackMode mode, RequestDeviceCallback* callback, UncapturedErrorCallback* errorCallback) {
    descriptor->Get().uncapturedErrorCallbackInfo.callback = [](const WGPUDevice* device, WGPUErrorType type, WGPUStringView message, void* callback_param, void* userdata_param) {
        UncapturedErrorCallback* cback = reinterpret_cast<UncapturedErrorCallback*>(callback_param);
        cback->OnCallback(type, WebGPUStringView(message).GetString().c_str());
    };
    descriptor->Get().uncapturedErrorCallbackInfo.userdata1 = reinterpret_cast<void*>(errorCallback);
    descriptor->Get().uncapturedErrorCallbackInfo.userdata2 = NULL;

    WGPURequestDeviceCallbackInfo callbackInfo = {};
    callbackInfo.mode = mode;
    callbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice dev, WGPUStringView message, void* callback_param, void*) {
        RequestDeviceCallback* cback = reinterpret_cast<RequestDeviceCallback*>(callback_param);
        WebGPUDevice* device = new WebGPUDevice();
        device->Set(dev);
        cback->OnCallback(status, device);
    };
    callbackInfo.userdata1 = reinterpret_cast<void*>(callback);
    callbackInfo.userdata2 = NULL;
    const WGPUDeviceDescriptor* desc = reinterpret_cast<WGPUDeviceDescriptor const*>(descriptor);
    auto result = wgpuAdapterRequestDevice(Get(), desc, callbackInfo);
}

bool WebGPUAdapter::GetInfo(WebGPUAdapterInfo* adapterInfo) {
    WGPUStatus status = wgpuAdapterGetInfo(Get(), &(adapterInfo->Get()));
    return status == WGPUStatus_Success;
}

bool WebGPUAdapter::HasFeature(WGPUFeatureName featureName) {
    return wgpuAdapterHasFeature(Get(), featureName);
}

WGPUStatus WebGPUAdapter::GetLimits(WebGPULimits* limits) {
    return wgpuAdapterGetLimits(Get(), &limits->Get());
}

// WebGPUSurface
void WebGPUSurface::AddRefInternal() {
    wgpuSurfaceAddRef(Get());
}

void WebGPUSurface::ReleaseInternal() {
    wgpuSurfaceRelease(Get());
}

void WebGPUSurface::Unconfigure() {
    wgpuSurfaceUnconfigure(Get());
}

void WebGPUSurface::Configure(WebGPUSurfaceConfiguration* config) {
    wgpuSurfaceConfigure(Get(), reinterpret_cast<WGPUSurfaceConfiguration const * >(config));
}

void WebGPUSurface::GetCapabilities(WebGPUAdapter* adapter, WebGPUSurfaceCapabilities* surfaceCapabilities) {
    wgpuSurfaceGetCapabilities(Get(), adapter->Get(), &(surfaceCapabilities->Get()));
}

void WebGPUSurface::GetCurrentTexture(WebGPUSurfaceTexture* tex) {
    wgpuSurfaceGetCurrentTexture(Get(), &tex->Get());
}

void WebGPUSurface::Present() {
    wgpuSurfacePresent(Get());
}

// WebGPUQueue
void WebGPUQueue::AddRefInternal() {
    wgpuQueueAddRef(Get());
}

void WebGPUQueue::ReleaseInternal() {
    wgpuQueueRelease(Get());
}

void WebGPUQueue::SetLabel(const char* value) {
    WebGPUStringView stringView(value);
    wgpuQueueSetLabel(Get(), stringView.Get());
}

void WebGPUQueue::Submit(int commandCount, WebGPUCommandBuffer* commandBuffer) {
    wgpuQueueSubmit(Get(), commandCount, &(commandBuffer->Get()));
}

void WebGPUQueue::WriteBuffer(WebGPUBuffer* buffer, int bufferOffset, WGPUByteBuffer* bytes) {
    int size = 0;
    void* data = NULL;
    if(bytes != NULL) {
        size = bytes->getLimit();
        data = (void*)bytes->data();
    }
    wgpuQueueWriteBuffer(Get(), buffer->Get(), bufferOffset, data, size);
}

void WebGPUQueue::WriteTexture(WebGPUTexelCopyTextureInfo* destination, WGPUByteBuffer* bytes, WebGPUTexelCopyBufferLayout* dataLayout, WebGPUExtent3D* writeSize) {
    int size = 0;
    void* data = NULL;
    if(bytes != NULL) {
        size = bytes->getLimit();
        data = (void*)bytes->data();
    }
    wgpuQueueWriteTexture(Get(), &destination->Get(), data, size, dataLayout->Get(), writeSize->Get());
}