#include <iostream>

#include <Precomp.h>
#include <TlkFileReader.h>

int main( int argc, char* argv[] )
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[ 0 ] << " <dialog.TLK file>" << std::endl;
        return 1;
    }

    using TlkFileReader16 = TlkFileReader<NWN::ResRef16>;

    const auto filepath = argv[ 1 ];
    TlkFileReader16 tlk( filepath );

    std::string strVal;
    for( TlkFileReader16::StrRef ref = 0; ref < 10; ++ref ) {
        if( tlk.GetTalkString( ref, strVal ) ) {
            std::cout << ref << ": " << strVal << std::endl;
        }
    }
}
