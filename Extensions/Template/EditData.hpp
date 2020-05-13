/* EditData.hpp
 * This is where you control what data
 * you want to have at edittime. You
 * are responsible for serializing and
 * deserializing (saving/loading) the
 * data to/from the SerializedED structure
 * as if it were a file on the hard drive.
 * It doesn't need to be efficient; this
 * is just at edittime and once at the
 * start of the runtime.
 */

#ifndef EDIF_PLAIN_EDITDATA
struct EditData final
{
	Vox* vox = nullptr;

	/* <default constructor>
	* This is where you provide default values for
	* your editdata. This constructor is used
	* when your extension is first created and
	* default values are needed.
	*/
	EditData() // : MyString(_T("Hello, world!")), MyInt(1337)
	{
		vox = new Vox();
	}

	/* <copy constructor>
	* As a convenience in other parts of your code,
	* you should copy data from another instance
	* of the EditData class. Make sure you deep-copy
	* dynamically allocated memory e.g. with pointers.
	*/
	EditData(const EditData &from)// : MyString(from.MyString), MyInt(from.MyInt), MyArray(from.MyArray)
	{

	}

	/* operator=
	* This is essentially the same as the copy
	* constructor above, except you are working
	* with an instance that is already
	* constructed.
	*/
	EditData &operator=(const EditData &from)
	{

	}

#ifndef RUN_ONLY
	/* Serialize
	* This is where you need to "write" data
	* to SerializedED like a file. Make sure
	* you can read the data back in the
	* constructor below!
	*/
	bool Serialize(mv *mV, SerializedED *&SED) const
	{
		EDOStream os(mV, SED);
		os.write_value<float>(vox->localAngleX);
		os.write_value<float>(vox->localAngleY);
		os.write_value<float>(vox->localAngleZ);
		os.write_value<float>(vox->localScale);
		os.write_value<unsigned int>(vox->width);
		os.write_value<unsigned int>(vox->height);
		os.write_value<int>(vox->hotSpotX);
		os.write_value<int>(vox->hotSpotY);
		os.write_value<int>(vox->hotSpotZ);
		os.write_value<bool>(vox->depthAsAlpha);
		os.write_string(vox->positionParameterX);
		os.write_string(vox->positionParameterY);
		os.write_string(vox->depthSizeParameter);
		os.write_value<bool>(vox->animateMovements);
		os.write_value<float>(vox->angleOffsetY);
		for (unsigned int i = 0; i < MaxAnimations; i++) {
			VoxAnimation* animation = &vox->voxAnimations[i];
			os.write_string(animation->filename);
			os.write_value<int>(animation->maxSpeed);
			os.write_value<int>(animation->minSpeed);
		}
		return true;
	}
#endif

	/* <constructor>
	* This is the primary constructor for the
	* EditData class. Here you will have to
	* "read" SerializedED like a file and
	* load back everything that gets saved
	* above in Serialize. Make sure you check
	* the version information first, this
	* constructor is used to update from
	* older versions of your editdata as well.
	*/
	EditData(SerializedED *SED, mv* mV, bool partial = false, Vox* runtimeVox = nullptr)
	{
		if (SED->Header.extVersion == 0) //older version
		{
			//code to update from an older version
		}
		else if (SED->Header.extVersion == 1) //current version
		{
			Vox* voxPtr;
			if (runtimeVox == nullptr) {
				vox = new Vox();
				voxPtr = vox;
			}
			else {
				voxPtr = runtimeVox;
			}
			EDIStream is(SED);
			voxPtr->localAngleX = is.read_value<float>();
			voxPtr->localAngleY = is.read_value<float>();
			voxPtr->localAngleZ = is.read_value<float>();
			voxPtr->localScale = is.read_value<float>();
			voxPtr->width = is.read_value<unsigned int>();
			voxPtr->height = is.read_value<unsigned int>();
			voxPtr->hotSpotX = is.read_value<int>();
			voxPtr->hotSpotY = is.read_value<int>();
			voxPtr->hotSpotZ = is.read_value<int>();
			voxPtr->depthAsAlpha = is.read_value<bool>();
			voxPtr->positionParameterX = is.read_string();
			voxPtr->positionParameterY = is.read_string();
			voxPtr->depthSizeParameter = is.read_string();
			voxPtr->animateMovements = is.read_value<bool>();
			voxPtr->angleOffsetY = is.read_value<float>();
			if (!partial) {
				int validAnimation = 0;
				for (unsigned int i = 0; i < MaxAnimations; i++)
				{
					VoxAnimation* animation = &voxPtr->voxAnimations[i];
					animation->filename = is.read_string();
					animation->maxSpeed = is.read_value<int>();
					animation->minSpeed = is.read_value<int>();
					if (animation->filename != "") {
						validAnimation = i;
					}
				}
				voxPtr->mV = mV;
				voxPtr->SetAnimation(validAnimation);
			}
		}
		else //the version is newer than current
		{
			//Either try to load the data anyway assuming your
			//future self was smart enough to keep the data in
			//the same format with new data at the end, or
			//make an error dialog and load some default data.
			MessageBox(NULL, _T("The MFA you are trying to load was saved")
				_T("with a newer version of this extension."),
				_T("Error Loading My Extension"), MB_OK);
		}
	}

	/* <destructor>
	* If you grabbed any memory e.g. with new,
	* make sure to e.g. delete it in here.
	*/
	~EditData()
	{
		delete vox;
	}
};
#else
struct EDITDATA
{
	Vox vox;
};
#endif