#include <iostream>

#include <Precomp.h>
#include <TlkFileReader.h>

int main( int argc, char* argv[] )
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[ 0 ] << " <CLASSES.2DA file>" << std::endl;
        return 1;
    }

    TlkFileReader<NWN::ResRef16> tlk( "" );
}
