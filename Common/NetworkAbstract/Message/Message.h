//
// Created by wurmel_a on 25/09/17.
//

#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <vector>

namespace NetworkAbstract {

    class Message {

#if !_WIN32
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
	public:

        enum { headerSize = sizeof(unsigned short) + sizeof(int) + sizeof(unsigned int) };
        enum { maxBodySize = 512 };
        enum { magic_number = 0x424D };

    public:
        Message();
        Message(Message const&);
        ~Message();

    public:
        void    setBody(const char *, unsigned int);
        char    *getBody();
        char const* getBody() const;
        void    *data();
        bool    decodeHeader();
		void	decodeData();
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

	template<typename T>
	T	getTypeOf(Message const& message) {
		return static_cast<T>(message.getType());
	}

}

#endif //NETWORK_MESSAGE_H
