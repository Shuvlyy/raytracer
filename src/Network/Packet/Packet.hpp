#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace raytracer::network
{

    namespace packet
    {

        enum class Type
            : uint8_t
        {
            UNKNOWN = 0x00,
            PING = 0x01,        /// | Server -> Client | Sent to get a slave's status: Latency, progress...
            PONG = 0x02,        /// | Client -> Server | Response to PING
            KISS = 0x03,        /// | Server -> Client | Sent to disconnect a client
            WORKSLAVE = 0x04,   /// | Server -> Client | Sent to notify a slave of a new render it has to do
            CESTCIAO = 0x05,    /// | Client -> Server | Sent when a slave disconnects
            FINITO = 0x06,      /// | Client -> Server | Sent when a slave has finished its render
            NVMSTOP = 0x07      /// | Server -> Client | Sent to stop a slave's rendering process
        };

    }

    using ByteBuffer = std::vector<uint8_t>;

    /**
     * Base class for all network packets.
     *
     * Defines the interface and common logic for serializing and
     * deserializing packets exchanged between the server and the client.
     */
    class Packet
    {
    public:
        /**
         * Creates a new Packet, with a specific type.
         *
         * @param   type    Type of packet to send
         */
        explicit Packet(packet::Type type);

        /**
         * Creates a Packet from an existing byte buffer.
         *
         * @param   buf Existing byte buffer
         */
        static std::unique_ptr<Packet> fromByteBuffer(const ByteBuffer& buf);

        virtual ~Packet() = default;

        /**
         * Serializes the packet into raw bytes for network transmission.
         *
         * @returns A vector of bytes representing the serialized packet
         */
        [[nodiscard]] virtual ByteBuffer serialize() const = 0;

        /**
         * Deserializes the packet from raw bytes received from the network.
         *
         * @param   data    Pointer to the raw byte data
         * @param   size    Size of the data buffer in bytes
         *
         * @throws  exception::UnexpectedRemainingData  If unexpected remaining
         *                                              data have been found
         */
        virtual void deserialize(
            const uint8_t* data,
            size_t size
        ) = 0;

        [[nodiscard]] packet::Type getType() const { return this->_type; }

        /**
         * @param   rawType Raw type in a single byte
         * @returns Converted type
         */
        static packet::Type fromRawTypeToType(const uint8_t rawType)
        {
            static const std::unordered_map<uint8_t, packet::Type> lookup =
            {
                { 0x01, packet::Type::PING },
                { 0x02, packet::Type::PONG },
                { 0x03, packet::Type::KISS },
                { 0x04, packet::Type::WORKSLAVE },
                { 0x05, packet::Type::CESTCIAO },
                { 0x06, packet::Type::FINITO },
                { 0x07, packet::Type::NVMSTOP },
            };

            const auto it = lookup.find(rawType);
            return it != lookup.end() ? it->second : packet::Type::UNKNOWN;
        }

        /**
         * @param   type    Type to get the name from
         * @returns Name of the packet type
         */
        static std::string fromTypeToString(const packet::Type type)
        {
            static const std::unordered_map<packet::Type, std::string> lookup =
            {
                { packet::Type::PING, "PING" },
                { packet::Type::PONG, "PONG" },
                { packet::Type::KISS, "KISS" },
                { packet::Type::WORKSLAVE, "WORKSLAVE" },
                { packet::Type::CESTCIAO, "CESTCIAO" },
                { packet::Type::FINITO, "FINITO" },
                { packet::Type::NVMSTOP, "NVMSTOP" },
            };

            const auto it = lookup.find(type);
            return it != lookup.end() ? it->second : "UNKNOWN";
        }

    protected:
        packet::Type _type;
    };

}
