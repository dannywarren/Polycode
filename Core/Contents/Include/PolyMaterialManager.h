/*
Copyright (C) 2011 by Ivan Safrin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
 
#pragma once
#include "PolyGlobals.h"
#include "PolyImage.h"
#include <vector>

class TiXmlNode;

namespace Polycode {
	
	class Cubemap;
	class Material;
	class PolycodeShaderModule;
	class Texture;
	class SceneRenderTexture;
	class Shader;
	class String;
	
	/**
	* Manages loading and reloading of materials, textures and shaders. This class should be only accessed from the CoreServices singleton.
	*/
	class _PolyExport MaterialManager {
		public:
			MaterialManager();
			~MaterialManager();
			
			void Update(int elapsed);

			/**
			* Creates a new framebuffer texture.
			*/ 
			Texture *createFramebufferTexture(int width, int height, int type);
			Texture *createTexture(int width, int height, char *imageData, bool clamp=false, bool createMipmaps = true, int type=Image::IMAGE_RGBA);
			Texture *createNewTexture(int width, int height, bool clamp=false, bool createMipmaps = true, int type=Image::IMAGE_RGBA);
			Texture *createTextureFromImage(Image *image, bool clamp=false, bool createMipmaps = true);
			Texture *createTextureFromFile(const String& fileName, bool clamp=false, bool createMipmaps = true);
			void deleteTexture(Texture *texture);
		
			void reloadTextures();
			
			void reloadProgramsAndTextures();
			void reloadPrograms();		
		
			void addShaderModule(PolycodeShaderModule *module);		
		
			//SceneRenderTexture *createRenderTexture(Scene *targetScene, Camera *targetCamera, int renderWidth,int renderHeight);
			Texture *getTextureByResourcePath(const String& resourcePath) const;
			
			// cubemaps
		
			Cubemap *cubemapFromXMLNode(TiXmlNode *node);
		
			// materials
			void loadMaterialsFromFile(String fileName);
			Material *materialFromXMLNode(TiXmlNode *node);
			Shader *setShaderFromXMLNode(TiXmlNode *node);
			Shader *createShaderFromXMLNode(TiXmlNode *node);
		
		private:
			std::vector<Texture*> textures;
			std::vector<Material*> materials;
		
			std::vector <PolycodeShaderModule*> shaderModules;
	};
};
