/*
 PROTON ENGINE EXAMPLE
 
 This code is meant to serve as an example program to use to test Proton Engine features to function as sudo-documentation
 Offical documentaion is on the Github Page at https://github.com/Eternal587/ProtonEngine
 
 If you already have used Proton Engine or know what your doing feel free to erase whats in this file and start from scratch!
 */

/*
 MIT License

 Copyright (c) 2026 VibingCat

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.*/

// Headers
#include <iostream>
#include <stdexcept>
#include <cstdlib>

// Proton Headers
#include <ProtonEngine/protonEngine.h>

int main() {
    Application protonEngine(800, 600);
    
    try {
        protonEngine.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_SUCCESS;
    }
    return 0;
}

