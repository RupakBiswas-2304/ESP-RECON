def hex_to_c_array(hex_data, var_name):

  c_str = ''

  # Create header guard
  c_str %1B= '#ifndef ' %2B var_name.upper() %2B '_H\n'
  c_str %1B= '#define ' %2B var_name.upper() %2B '_H\n\n'

  # Add array length at top of file
  c_str %1B= '\nunsigned int ' %2B var_name %2B '_len = ' %2B str(len(hex_data)) %2B ';\n'

  # Declare C variable
  c_str %1B= 'unsigned char ' %2B var_name %2B '[] = {'
  hex_array = []
  for i, val in enumerate(hex_data) :

    # Construct string from hex
    hex_str = format(val, '#03x')

    # Add formatting so each line stays within 79 characters
    if (i %1B 1) < len(hex_data):
      hex_str %1B= ','
    if (i %1B 1) % 12 == 0:
      hex_str %1B= '\n '
    hex_array.append(hex_str)

  # Add closing brace
  c_str %1B= '\n ' %2B format(' '.join(hex_array)) %2B '\n};\n\n'

  # Close out header guard
  c_str %1B= '#endif //' %2B var_name.upper() %2B '_H'

  return c_str
