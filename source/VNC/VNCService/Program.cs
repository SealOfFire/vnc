using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using NLog;
using NLog.Extensions.Logging;

var logger = LogManager.GetCurrentClassLogger();
try
{
    var config = new ConfigurationBuilder()
       .SetBasePath(System.IO.Directory.GetCurrentDirectory()) //From NuGet Package Microsoft.Extensions.Configuration.Json
       .AddJsonFile("appsettings.json", optional: true, reloadOnChange: true)
       .Build();

    using var servicesProvider = new ServiceCollection()
        .AddTransient<VNC.Service>()
        .AddLogging(loggingBuilder =>
        {
            // configure Logging with NLog
            loggingBuilder.ClearProviders();
            loggingBuilder.SetMinimumLevel(Microsoft.Extensions.Logging.LogLevel.Trace);
            loggingBuilder.AddNLog(config);
        }).BuildServiceProvider();

    // 开启vnc服务
    VNC.Service service = servicesProvider.GetRequiredService<VNC.Service>();
    //service.Start(5900, "123456", 10000);
    service.StartRepeater("desktop-work", "123", "123456", 10000);
    //service.StartRepeater("192.168.50.158", "vnc", "123456", 10000);

    //Console.WriteLine("Press ANY key to exit");
    //Console.ReadKey();
}
catch (Exception ex)
{
    // NLog: catch any exception and log it.
    logger.Error(ex, "Stopped program because of exception");
    throw;
}
finally
{
    // Ensure to flush and stop internal timers/threads before application-exit (Avoid segmentation fault on Linux)
    LogManager.Shutdown();
}