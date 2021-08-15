# Summary

## API

AssetManager types and functions are defined in the ge2::ast namespace.

### AssetManager.h

**Mode**  
Passed into the initialize function to determine certain aspects of asset manager functionality. Allows client code to decide whether assets should all be loaded in bulk or only when required.

**void Initialize(Mode)**  
Must be called before using any other AssetManager functions. Loads in metadata, and may load in assets based on the *Mode* that is passed in.

### Loader.h



### Metadata.h

**struct Metadata**  
Contains various information about a particular asset:
 * GUID
 * Name
 * Dependencies
 * Path
 * Extension
 * Missing

**Metadata\* GetMetatdata(GUID)**  
Access the metadata for the asset associated with the GUID. Returns *nullptr* if no metadata exists which matches the GUID.

**Metadata\* GetMetatdata(std::wstring const&)**  
Access the metadata for the asset associated with the filename. Returns *nullptr* if no metadata exists which matches the filename.

**GUID GuidFromName(std::wstring const&)**  
Find the GUID of a specific file from its name. Filenames should be used sparingly for names which must be hard coded, for example an initial scene to load into on startup.

**std::wstring NameFromGUID(GUID)**  

Find the name of a specific file from its GUID. Useful for displaying relevant names in an editor or in error messages. GUIDs should be preferred where possible.

