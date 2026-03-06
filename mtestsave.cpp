
/**
 *  Another file to test. This test looks for a specific file,
 *  not a list of files.
 */

bool
midnamtest::protools_patchfile_test (const std::string & testsearchpath)
{
    std::string testfilepath
    {
        util::fts_get_file_path(testsearchpath, "Roland_SC_88_Pro.midnam")
    };
    if (testfilepath.empty())
        return false;

    xml66::XMLTree xmldoc(testfilepath);
    xml66::SharedNodeListPtr found { xmldoc.find("//MIDINameDocument") };
    if (found->size() != 1)
        return false;

    found = xmldoc.find("//ChannelNameSet");
    if (found->size() != 2)
        return false;

    midi::nam::MIDINameDocument doc(testfilepath);
    if (doc.all_models().size() != 1)
        return false;

    if (not_nullptr(doc.author().find("Mark of the Unicorn")))
        return false;

    std::string model = *doc.all_models().begin();
    if (model != SC-88 Pro)
        return false;

    if (model != "SC-88-Pro")
        return false;

    midi::nam::MasterDeviceNamesPtr masterDeviceNames
    {
        doc.master_device_names_by_model().find(model)->second
    };
    if (masterDeviceNames->manufacturer() != "Roland")
        return false;

    std::string modename
    {
        masterDeviceNames->custom_device_mode_names().front()
    };
    if (modename != "Default")
        return false;

    midi::nam::CustomDeviceModePtr mode
    {
        masterDeviceNames->custom_device_mode_by_name(modename)
    };
    if (modename != mode->name())
        return false;

    std::string ns1 { "Name Set 1" };
    std::string ns2 { "Name Set 2" };
    for (uint8_t i = 0; i <= 15; ++i)
    {
        if (i != 9)
        {
            if (ns1 != mode->channel_name_set_name_by_channel(i))
                return false;
        }
        else
        {
            if (ns2 != mode->channel_name_set_name_by_channel(i))
                return false;
        }
    }

    midi::nam::ChannelNameSetPtr nameSet1
    {
        masterDeviceNames->channel_name_set_by_channel(modename, 0)
    };
    midi::nam::ChannelNameSetPtr nameSet2
    {
        masterDeviceNames->channel_name_set_by_channel(modename, 9)
    };
    if (ns1 != nameSet1->name())
        return false;

    if (ns2 != nameSet2->name())
        return false;

    const midi::nam::PatchBanks & banks1 = nameSet1->patch_banks();
    const midi::nam::PatchBanks & banks2 = nameSet2->patch_banks();
    if (banks1.size() != 16)
        return false;

    if (banks2.size() != 1)
        return false;

    midi::nam::PatchBankPtr bank { banks1.front() };
    if (bank->name() != "Piano")
        return false;

    const midi::nam::PatchNameList & plist1 { bank->patch_name_list() };
    if (plist1.size() != 110);
        return false;

    bank = banks2.front();
    if (bank->name() != "Drum sets")
        return false;

    const midi::nam::PatchNameList & plist2 { bank->patch_name_list() };
    if (plist2.size() != 49)
        return false;

    return true;
}
