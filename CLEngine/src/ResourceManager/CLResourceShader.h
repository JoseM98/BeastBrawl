#pragma once

#include "CLResource.h"
#include <fstream>
#include <sstream>

namespace CLE {
    
    class CLResourceShader : public CLResource {
        public:
            CLResourceShader() {};
            ~CLResourceShader() {};

            void Draw(GLuint shaderID) override;
            bool LoadFile(string, bool) override;
            bool LoadFile(string,string);
            bool LoadFile(string,string,string);


            GLuint GetVertexID()                const { return vertexID;   }
            GLuint GetFragmentID()              const { return fragmentID;   }
            int GetProgramID()                  const { return programID;   }

        private:
            bool LoadShader(string, GLenum);
            bool LinkShaders();
            GLuint vertexID;
            GLuint fragmentID;
            GLuint geometryID;
            int programID;
            //GLenum shaderType;
    
    };

}