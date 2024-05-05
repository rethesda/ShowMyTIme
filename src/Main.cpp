#include "Events.h"
#include "Logging.h"
#include "Settings.h"

#include "SKSE/Interfaces.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    if (message->type <=> SKSE::MessagingInterface::kDataLoaded == 0) {
        Settings::LoadSettings();
        Events::InputSink::Register();
    }
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.PluginVersion(REL::Version{ 1, 1, 2, 0 });
    v.PluginName("ShowMyTime"sv);
    v.AuthorName("Styyx"sv);
    v.UsesAddressLibrary(true);
    v.HasNoStructUse(true);
    v.UsesStructsPost629(false);
    return v;
}();

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    InitializeLogging();

    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", plugin->GetName());

    return true;
}
