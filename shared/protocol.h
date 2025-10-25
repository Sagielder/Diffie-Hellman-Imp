#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <cstdint>
#include <cstring>

enum class MessageType : uint8_t {
    INVALID = 0x00,
    REGISTER = 0x01,
    MESSAGE = 0x02,
    DH_INIT = 0x03,
    DH_RESPONSE = 0x04,
    ERROR_MSG = 0x05,
    USER_LIST = 0x06,
    DISCONNECT = 0x07
};

namespace MessageFlags {
    const uint8_t NONE = 0x00;
    const uint8_t ENCRYPTED = 0x01;
}

/*
Message Format
<uint32 total length><1 byte message type><1 byte encrypted flag>
<uint32 from_string length><n-bytes from_string><uint32 to_string length><n-bytes to_string>
<uint32 payload length><n-bytes payload>

fixed size = 4 + 1 + 1 + 4 + 4 + 4 = 18
*/

struct Message {
    MessageType type = MessageType::INVALID;    // 1-byte
    uint8_t flags = MessageFlags::NONE;         // 1-bytes
    std::string from;                           // n-bytes
    std::string to;                             // n-bytes
    std::string payload;                        // n-bytes
    
    bool isEncrypted() const {
        return flags == MessageFlags::ENCRYPTED;
    }
    
    std::vector<uint8_t> serialize() const {
        std::vector<uint8_t> buffer;
        
        uint32_t total_size = 18 + from.size() + to.size() + payload.size();
        
        buffer.reserve(total_size);
        
        writeUint32(buffer, total_size);
        
        buffer.push_back(static_cast<uint8_t>(type));
        
        buffer.push_back(flags);

        writeUint32(buffer, from.size());
        
        buffer.insert(buffer.end(), from.begin(), from.end());

        writeUint32(buffer, to.size());
        
        buffer.insert(buffer.end(), to.begin(), to.end());
         
        writeUint32(buffer, payload.size());
        
        buffer.insert(buffer.end(), payload.begin(), payload.end());
        
        return buffer;
    }
    
    static Message deserialize(const uint8_t* data, size_t data_size) {
        Message msg;
        
        if (data_size < 18) {
            msg.type = MessageType::INVALID;
            return msg;
        }
        
        size_t offset = 0;
        
        // read total size
        uint32_t total_size = readUint32(data, offset);
        offset += 4;
        
        if (total_size != data_size) {
            msg.type = MessageType::INVALID;
            return msg;
        }
        
        // read message type
        msg.type = static_cast<MessageType>(data[offset]);
        offset += 1;

        // read encrypted flag
        msg.flags = data[offset];
        offset += 1;
        
        // read from string
        uint32_t from_length = readUint32(data, offset);
        offset += 4;
        
        if (offset + from_length > data_size)
        {
            msg.type = MessageType::INVALID;
            return msg;
        }
        msg.from = std::string(reinterpret_cast<const char*>(data + offset), from_length);
        offset += from_length;

        // read to string
        uint32_t to_length = readUint32(data, offset);
        offset += 4;
        
        if (offset + to_length > data_size) {
            msg.type = MessageType::INVALID;
            return msg;
        }
        
        msg.to = std::string(reinterpret_cast<const char*>(data + offset), to_length);
        offset += to_length;
        
        
        // read payload
        uint32_t payload_length = readUint32(data, offset);
        offset += 4;
        
        if (offset + payload_length != data_size) {
            msg.type = MessageType::INVALID;
            return msg;
        }
        
        msg.payload = std::string(reinterpret_cast<const char*>(data + offset), payload_length);
        
        return msg;
    }
    
    
    
private:
    // write in little-endian
    static void writeUint32(std::vector<uint8_t>& buffer, uint32_t value) {
        buffer.push_back((value) & 0xFF);
        buffer.push_back((value >> 8) & 0xFF);
        buffer.push_back((value >> 16) & 0xFF);
        buffer.push_back((value >> 24) & 0xFF);
    }

    static void writeUint32(char* buffer, const uint32_t& value, const uint32_t& offset)
    {
        buffer[offset] = (value) & 0xFF;
        buffer[offset + 1] = (value >> 8) & 0xFF;
        buffer[offset + 2] = (value >> 16) & 0xFF;
        buffer[offset + 3] = (value >> 24) & 0xFF;
    }
    
    // read in little-endian
    static uint32_t readUint32(const uint8_t* data, size_t offset) {
        return (static_cast<uint32_t>(data[offset])) |
               (static_cast<uint32_t>(data[offset + 1]) << 8) |
               (static_cast<uint32_t>(data[offset + 2]) << 16) |
               static_cast<uint32_t>(data[offset + 3] << 24);
    }
};

class MessageReceiver {
private:
    std::vector<uint8_t> buffer;
    size_t expected_size = 0;
    bool message_size_known = false;
    
public:
    // add data to buffer
    // return true when all data is received
    bool addData(const uint8_t* data, size_t length) {
        buffer.insert(buffer.end(), data, data + length);
        
        // first 4 bytes is the total message length
        if (!message_size_known && buffer.size() >= 4) {
            expected_size = Message::deserialize(buffer.data(), buffer.size()).serialize().size();
            expected_size = (static_cast<uint32_t>(buffer[0]) << 24) |
                          (static_cast<uint32_t>(buffer[1]) << 16) |
                          (static_cast<uint32_t>(buffer[2]) << 8) |
                          static_cast<uint32_t>(buffer[3]);
            message_size_known = true;
        }
        
        // check if all data is received
        if (message_size_known && buffer.size() >= expected_size) {
            return true;
        }
        
        return false;
    }
    
    Message getMessage() {
        Message msg = Message::deserialize(buffer.data(), expected_size);
        
        reset();
        
        return msg;
    }
    
    void reset() {
        buffer.clear();
        buffer.shrink_to_fit();
        expected_size = 0;
        message_size_known = false;
    }
};

#endif