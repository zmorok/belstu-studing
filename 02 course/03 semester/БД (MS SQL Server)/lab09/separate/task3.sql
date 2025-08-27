print 'ROWCOUNT: ' + convert(varchar, @@rowcount)
print 'VERSION: ' + version
print 'SPID: ' + convert(varchar, @@spid)
print 'ERROR: ' + convert(varchar, error)
print 'SERVERNAME: ' + @@servername
print 'TRANCOUNT: ' + convert(varchar, @@trancount)
print 'FETCH_STATUS: ' + convert(varchar, @@fetch_status)
print 'NESTLEVEL: ' + convert(varchar, @@nestlevel)