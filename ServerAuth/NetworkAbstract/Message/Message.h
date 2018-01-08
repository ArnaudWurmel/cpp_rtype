//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <vector>

namespace NetworkAbstract {
    class Message {
    public:
        enum    MessageType {
            Unknown = 0,
            Connect = 1,
            Error = 2,
            Userlist = 3,
            ChannelList = 4,
            Join = 5,
            Event = 6,
            Leave = 7,
            CreateChannel = 8
        };


#ifdef __linux__	
	struct __attribute__((packed)) AMessage {
            unsigned short magicNumber;
            int type;
            unsigned int bodySize;
            char    *body;
        };

#else
	#include "packed.h"
	struct AMessage {
            unsigned short magicNumber;
            int type;
            unsigned int bodySize;
            char    *body;
        } PACKED;
	#include "endpacked.h"
#endif

        enum { headerSize = sizeof(unsigned short) + sizeof(MessageType) + sizeof(unsigned int) };
        enum { maxBodySize = 512 };
        enum { magic_number = 0x424D };

    public:
        Message(MessageType const&);
        Message(Message const&);
        ~Message();

    public:
        void    setBody(const char *, unsigned int);
        char    *getBody();
        char const* getBody() const;
        void    *data();
        bool    decodeHeader();
        bool    encodeHeader();
        void    encodeData();
        unsigned int    getBodySize() const;
        unsigned int    totalSize() const;
        int getType() const;
        void    setType(int);

    private:
        unsigned int    _bodySize;
        AMessage    _message;
        std::vector<char>   _data;
    };
}

#endif //NETWORK_MESSAGE_H
